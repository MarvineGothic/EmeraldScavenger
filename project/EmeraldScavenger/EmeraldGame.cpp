#include <memory>

#include <sre/Inspector.hpp>
#include "EmeraldGame.hpp"
#include "GameObject.hpp"
#include "sre/RenderPass.hpp"
#include "sre/Texture.hpp"
#include "SpriteComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "PhysicsComponent.hpp"
#include "Player.hpp"
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include <windows.h>

static const string platform = "Windows";
#else
static const std::string platform = "Mac";
#include <sys/time.h>
timeval macTime;
auto currentTime = gettimeofday(&macTime, NULL);
auto time_ms = (macTime.tv_sec * 1000) + (macTime.tv_usec / 1000);
#endif

const vec2 EmeraldGame::windowSize(800, 600);
const vec2 EmeraldGame::scale(0.2f, 0.2f);
EmeraldGame *EmeraldGame::gameInstance = nullptr;

EmeraldGame::EmeraldGame()
        : debugDraw(physicsScale) {
    bool useVsync = true;
    gameInstance = this;
    r.setWindowSize(windowSize);
    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL)
            .withVSync(useVsync);

    spriteAtlas = SpriteAtlas::create("obstacles.json", Texture::create()
            .withFile("obstacles.png")
            .withFilterSampling(false)
            .build());
    enemiesAtlas = SpriteAtlas::create("enemiesSprites.json", Texture::create()
            .withFile("enemiesSprites.png")
            .withFilterSampling(false)
            .build());
    spriteAtlas_02 = SpriteAtlas::create("gameSprites.json", Texture::create()
            .withFile("gameSprites.png")
            .withFilterSampling(false)
            .build());
    scavengerAtlas = SpriteAtlas::create("scavangerStages.json", Texture::create()
            .withFile("scavangerStages.png")
            .withFilterSampling(false)
            .build());
    uiAtlas = SpriteAtlas::create("ui.json", Texture::create()
            .withFile("ui.png")
            .withFilterSampling(false)
            .build());
    level = Level::createDefaultLevel(this, spriteAtlas, enemiesAtlas);

    //Get the current time in milliseconds and use it as a seed
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    SYSTEMTIME windowsTime;
    GetSystemTime(&windowsTime);
    auto time_ms = (windowsTime.wSecond * 1000) + windowsTime.wMilliseconds;
#endif
    int rand_seed = (time_t) time_ms;
    printf("Random seed: %ld\n", rand_seed);
    srand(rand_seed);

    initGame();

    if (gameState != GameState::Pause) {
        // setup callback functions
        r.keyEvent = [&](SDL_Event &e) {
            onKey(e);
        };
        r.frameUpdate = [&](float deltaTime) {
            update(deltaTime);
        };
        r.frameRender = [&]() {
            render();
        };
        // start game loop
        r.startEventLoop();
    }
}

// ============================================ INIT FUNCTIONS =========================================================

void EmeraldGame::initGame() {
    if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
    }
    resetGame();
    initCamera();
}

void EmeraldGame::runGame() {
    initGame();
    initPhysics();
    initLevel();
    initPlayer(level->getStartPos());
    gameState = GameState::Running;
}

void EmeraldGame::resetGame() {
    player.reset();
    camera.reset();
    background.resetBackground();
    gameObjectsList.clear();
    physicsComponentMap.clear();
}

void EmeraldGame::initCamera() {
    auto camObj = createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize * 0.5f);
    auto camPos = camObj->getPosition();
}

void EmeraldGame::initPhysics() {
    float gravity = -9.8f; // 9.8 m/s2
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);

    if (doDebugDraw)
        world->SetDebugDraw(&debugDraw);
}

void EmeraldGame::initLevel() {
    level->makeLevel(levelCounter);
}

void EmeraldGame::initPlayer(vec2 position) {
    player = createGameObject();
    player->name = "Player";
    auto playerSpriteComponent = player->addComponent<SpriteComponent>();
    auto playerSpriteObj = scavengerAtlas->get("boy-idle.png");
    playerSpriteObj.setPosition(position);
    playerSpriteObj.setScale(EmeraldGame::scale);
    playerSpriteComponent->setSprite(playerSpriteObj);
    auto playerComponent = player->addComponent<Player>();
    playerComponent->setSprites(
            scavengerAtlas->get("boy-idle.png"),
            scavengerAtlas->get("jump_upboy.png"),
            scavengerAtlas->get("jump_fallboy.png"),
            scavengerAtlas->get("frame-1_boy.png"),
            scavengerAtlas->get("frame-2_boy.png"),
            scavengerAtlas->get("frame-3_boy.png"),
            scavengerAtlas->get("boy-hit.png")
    );
    // set camera follow player:
    camera->setFollowObject(player, {windowSize * 0.5f});
}

// ============================================ GAME LOOP FUNCTIONS ====================================================
void EmeraldGame::onKey(SDL_Event &event) {
    if (gameState != GameState::Pause)
        for (auto &gameObject : gameObjectsList) {
            for (auto &c : gameObject->getComponents()) {
                bool consumed = c->onKey(event);
                if (consumed) {
                    return;
                }
            }
        }

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            // when game-over - any button lead to start menu
            case SDLK_a:
                if (gameState == GameState::GameOver)
                    gameState = GameState::Start;
                break;

            case SDLK_z:
                camera->setZoomMode(!camera->isZoomMode());
                break;
            case SDLK_d:
                // press 'd' for physics debug
                doDebugDraw = !doDebugDraw;
                if (doDebugDraw) {
                    world->SetDebugDraw(&debugDraw);
                } else {
                    world->SetDebugDraw(nullptr);
                }
                break;
                // in start menu press space to play;
            case SDLK_SPACE:
                if (gameState == GameState::Ready)
                    runGame();
                break;
            case SDLK_p:
                if (gameState == GameState::Running) {
                    gameState = GameState::Pause;
                } else if (gameState == GameState::Pause) {
                    gameState = GameState::Running;

                }
                break;
            default:
                break;
        }
    }
}

void EmeraldGame::update(float time) {

    if (gameState == GameState::Running) {
        updatePhysics();
        if (time > 0.03) // if framerate approx 30 fps then run two physics steps
            updatePhysics();
    }
    for (auto &gameObject : gameObjectsList) {
        gameObject->update(time);
    }
    if (gameState == GameState::Running) {
        if (player->getComponent<Player>()->lostLife) {
            livesCounter--;
            player->getComponent<Player>()->lostLife = false;
            setGameState(GameState::GetReady);
        }
        if (livesCounter < 1)
            gameState = GameState::GameOver;
    }
    if (gameState == GameState::GetReady) {
        if (player->getComponent<Player>()->inPit) {
            player->getComponent<Player>()->inPit = false;
            runGame();
        }
        setGameState(GameState::Running);
    }
}

void EmeraldGame::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera->getCamera())
            .build();

    if (doDebugDraw) {
        static Inspector profiler;
        profiler.update();
        profiler.gui(false);
    }

    background.renderBackground(renderPass, 0.0f);

    auto pos = camera->getGameObject()->getPosition();
    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto &go : gameObjectsList) {
        go->renderSprite(spriteBatchBuilder);
    }

    // ================================ GAME STATES ================================
    // ================ by: Sergiy Isakov 02 - 03.11.18 ============================
    if (gameState == GameState::GameOver) {
        resetGame();
        initCamera();
        auto sprite = spriteAtlas_02->get("spr_gameOver.png");
        sprite.setPosition({0.0f, 0.0f});
        spriteBatchBuilder.addSprite(sprite);
    } else if (gameState == GameState::Start) {
        gameState = GameState::Ready;
        livesCounter = 5;
        emeraldCounter = 0;
    } else if (gameState == GameState::Ready) {
        auto sprite = spriteAtlas->get("diamond blue.png");
        sprite.setPosition({0.0f, 0.0f});
        spriteBatchBuilder.addSprite(sprite);
    } else if (gameState == GameState::Pause) {
        pauseSprite = spriteAtlas_02->get("spr_paused.png");
        pauseSprite.setPosition(pos);
        spriteBatchBuilder.addSprite(pauseSprite);
    }

    // ======================== LIVES AND EMERALDS ==================================
    // ===================== by: Sergiy Isakov 17.11.18 22.17 =======================
    if (gameState == GameState::Running || gameState == GameState::Pause) {
        auto livesSprite = spriteAtlas_02->get("spr_heart" + to_string(livesCounter) + ".png");
        livesSprite.setPosition({pos.x - windowSize.x * 0.4f, pos.y + windowSize.y * 0.45f});
        livesSprite.setScale(EmeraldGame::scale * 1.5f);
        spriteBatchBuilder.addSprite(livesSprite);

        auto emeraldSprite = spriteAtlas->get("diamond blue.png");
        emeraldSprite.setScale(EmeraldGame::scale * 0.8f);
        emeraldSprite.setPosition({pos.x + windowSize.x * 0.45f, pos.y + windowSize.y * 0.45f});
        spriteBatchBuilder.addSprite(emeraldSprite);

        auto emeraldNumber = uiAtlas->get("numeral" + to_string(emeraldCounter) + ".png");
        emeraldNumber.setPosition({emeraldSprite.getPosition().x - emeraldNumber.getSpriteSize().x * 2.0f,
                                   emeraldSprite.getPosition().y});
        spriteBatchBuilder.addSprite(emeraldNumber);
    }
    // ==============================================================================

    auto sb = spriteBatchBuilder.build();
    renderPass.draw(sb);

    if (doDebugDraw) {
        world->DrawDebugData();
        renderPass.drawLines(debugDraw.getLines());
        debugDraw.clear();
    }
}

// ============================================ HELPER FUNCTIONS =======================================================
shared_ptr<GameObject> EmeraldGame::createGameObject() {
    auto obj = make_shared<GameObject>();
    gameObjectsList.push_back(obj);
    return obj;
}

void EmeraldGame::deleteGameObject(shared_ptr<GameObject> gameObject) {
    gameObjectsList.erase(remove(gameObjectsList.begin(), gameObjectsList.end(), gameObject), gameObjectsList.end());
}

void EmeraldGame::updatePhysics() {

    const int positionIterations = 4;
    const int velocityIterations = 12;
    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto phys : physicsComponentMap) {
        PhysicsComponent *physicsComponent = phys.second;
        if (!physicsComponent->isAutoUpdate()) continue;
        auto position = physicsComponent->getBody()->GetPosition();
        float angle = physicsComponent->getBody()->GetAngle();
        auto gameObject = physicsComponent->getGameObject();
        gameObject->setPosition(vec2(position.x * physicsScale, position.y * physicsScale));
        gameObject->setRotation(angle);
    }
}

void EmeraldGame::BeginContact(b2Contact *contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void EmeraldGame::EndContact(b2Contact *contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

void EmeraldGame::deregisterPhysicsComponent(PhysicsComponent *r) {
    auto iter = physicsComponentMap.find(r->getFixture());
    if (iter != physicsComponentMap.end()) {
        physicsComponentMap.erase(iter);
    } else {
        assert(false); // cannot find physics object
    }
}

void EmeraldGame::registerPhysicsComponent(PhysicsComponent *r) {
    physicsComponentMap[r->getFixture()] = r;
}

void EmeraldGame::handleContact(b2Contact *contact, bool begin) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentMap.find(fixA);
    auto physB = physicsComponentMap.find(fixB);
    if (physA != physicsComponentMap.end() && physB != physicsComponentMap.end()) {
        auto &aComponents = physA->second->getGameObject()->getComponents();
        auto &bComponents = physB->second->getGameObject()->getComponents();
        for (auto &c : aComponents) {
            if (begin) {
                c->onCollisionStart(physB->second);
            } else {
                c->onCollisionEnd(physB->second);
            }
        }
        for (auto &c : bComponents) {
            if (begin) {
                c->onCollisionStart(physA->second);
            } else {
                c->onCollisionEnd(physA->second);
            }
        }
    }
}

shared_ptr<Level> EmeraldGame::getLevel() {
    return this->level;
}

void EmeraldGame::setGameState(GameState newState) {
    this->gameState = newState;
}



