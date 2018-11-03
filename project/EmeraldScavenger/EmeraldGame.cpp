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
#include "BirdMovementComponent.hpp"
#include <time.h>
#include <windows.h>

const vec2 EmeraldGame::windowSize(800, 600);
EmeraldGame *EmeraldGame::gameInstance = nullptr;

EmeraldGame::EmeraldGame()
        : debugDraw(physicsScale) {
    bool useVsync = true;
    gameInstance = this;
    r.setWindowSize(windowSize);
    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL)
            .withVSync(useVsync);

    spriteAtlas = SpriteAtlas::create("platformer-art-deluxe.json", Texture::create()
            .withFile("platformer-art-deluxe.png")
            .withFilterSampling(false)
            .build());
    spriteAtlas_02 = SpriteAtlas::create("gameSprites.json", Texture::create()
            .withFile("gameSprites.png")
            .withFilterSampling(false)
            .build());
    level = Level::createDefaultLevel(this, spriteAtlas);

	// Initiate random seed based on time in milliseconds
	SYSTEMTIME time;
	GetSystemTime(&time);
	LONG time_ms = (time.wSecond * 1000) + time.wMilliseconds;
	time_t rand_seed = (time_t)time_ms;
	printf("Random seed: %d\n", rand_seed);
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
    auto playerSprite = player->addComponent<SpriteComponent>();
    auto playerSpriteObj = spriteAtlas->get("19.png");
    playerSpriteObj.setPosition(position);
    playerSprite->setSprite(playerSpriteObj);
    auto playerComponent = player->addComponent<Player>();
    playerComponent->setSprites(
            spriteAtlas->get("19.png"),
            spriteAtlas->get("20.png"),
            spriteAtlas->get("21.png"),
            spriteAtlas->get("26.png"),
            spriteAtlas->get("27.png"),
            spriteAtlas->get("28.png")
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
    if (gameState == GameState::GetReady) {
        if (livesCounter < 1) {
            gameState = GameState::GameOver;
        } else
            runGame();
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

    auto pos = camera->getGameObject()->getPosition();
    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto &go : gameObjectsList) {
        go->renderSprite(spriteBatchBuilder);
    }

    if (gameState == GameState::GameOver) {
        resetGame();
        initCamera();
        auto sprite = spriteAtlas_02->get("spr_gameOver.png");
        sprite.setPosition({0.0f, 0.0f});
        spriteBatchBuilder.addSprite(sprite);
    }
    if (gameState == GameState::Start) {
        /*auto sprite = spriteAtlas_02->get("spr_gameOver.png");
        sprite.setPosition({ 0.0f,0.0f });
        spriteBatchBuilder.addSprite(sprite);*/
        gameState = GameState::Ready;
        livesCounter = 5;
    }
    if (gameState == GameState::Pause) {
        pauseSprite = spriteAtlas_02->get("spr_paused.png");
        pauseSprite.setPosition(pos);
        spriteBatchBuilder.addSprite(pauseSprite);
    }

    background.renderBackground(renderPass, 0.0f);
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

