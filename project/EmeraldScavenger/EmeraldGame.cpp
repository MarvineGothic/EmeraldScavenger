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

int EmeraldGame::currentLevel = 0;
int EmeraldGame::nextLevel = 1;
vec2 EmeraldGame::currentStartPosition = vec2{NULL, NULL};
vec2 EmeraldGame::nextStartPosition = vec2{NULL, NULL};
const vec2 EmeraldGame::windowSize(800, 600);
const vec2 EmeraldGame::scale(0.2f, 0.2f);
EmeraldGame *EmeraldGame::gameInstance = nullptr;
bool EmeraldGame::introCleared = false, EmeraldGame::physCleared = false,
        EmeraldGame::gravCleared = false, EmeraldGame::procCleared = false,
        EmeraldGame::bonusCleared = false, canEnterNextLevel = true;

EmeraldGame::EmeraldGame()
        : debugDraw(physicsScale) {
    bool useVsync = true;
    gameInstance = this;
    r.setWindowSize(windowSize);
    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL)
            .withVSync(useVsync);

    //Get the current time in milliseconds and use it as a seed
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    SYSTEMTIME windowsTime;
    GetSystemTime(&windowsTime);
    auto time_ms = (windowsTime.wSecond * 1000) + windowsTime.wMilliseconds;
#endif
    int rand_seed = (time_t) time_ms;
    printf("Random seed: %ld\n", rand_seed);
    srand(rand_seed);

    initAssets();
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

void EmeraldGame::initAssets() {
    platformerArtAtlas = SpriteAtlas::create("platformer-art-deluxe.json", Texture::create()
            .withFile("platformer-art-deluxe.png")
            .withFilterSampling(false)
            .build());
    gameSpritesAtlas = SpriteAtlas::create("gameSprites.json", Texture::create()
            .withFile("gameSprites.png")
            .withFilterSampling(false)
            .build());
    obstaclesAtlas = SpriteAtlas::create("obstacles.json", Texture::create()
            .withFile("obstacles.png")
            .withFilterSampling(false)
            .build());
    uiAtlas = SpriteAtlas::create("ui.json", Texture::create()
            .withFile("ui.png")
            .withFilterSampling(false)
            .build());
    level = Level::createDefaultLevel(this);

    // init sprites
    gameOverSprite = gameSpritesAtlas->get("spr_gameOver.png");
    gameOverSprite.setPosition({0.0f, 0.0f});
    pauseSprite = gameSpritesAtlas->get("spr_paused.png");
    levelSprite = gameSpritesAtlas->get("level.png");
    levelSprite.setPosition({-50.0f, 0.0f});
    emeraldSprite = obstaclesAtlas->get("emerald.png");
    emeraldSprite.setScale(EmeraldGame::scale * 0.8f);

    audioManager = AudioManager::instance();
}

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
    initPlayer();
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
    level->makeLevel(currentLevel);
}

void EmeraldGame::initPlayer() {
    player = createGameObject();
    player->name = "Player";
    auto playerComponent = player->addComponent<Player>();

    playerComponent->setSprites(
            obstaclesAtlas->get("idle1.png"),
            obstaclesAtlas->get("jump1.png"),
            obstaclesAtlas->get("jump2.png"),
            obstaclesAtlas->get("running1.png"),
            obstaclesAtlas->get("running2.png"),
            obstaclesAtlas->get("running3.png"),
            obstaclesAtlas->get("hit.png")
    );

    // set camera follow player:
    camera->setFollowObject(player, {windowSize * 0.5f});
}

void EmeraldGame::initVariables() {
	livesCounter = 5;
	emeraldCounter = 0;
	currentLevel = 0;
	currentStartPosition = vec2{ NULL, NULL };
	EmeraldGame::introCleared = false, EmeraldGame::physCleared = false,
		EmeraldGame::gravCleared = false, EmeraldGame::procCleared = false,
		EmeraldGame::bonusCleared = false, canEnterNextLevel = true;
}

void EmeraldGame::completedLevel(int level) {
    switch (level) {
        case 0:
            introCleared = true;
            break;
        case 2:
            gravCleared = true;
            break;
        case 3:
            physCleared = true;
            break;
        case 4:
            procCleared = true;
            break;
        case 8:
            bonusCleared = true;
            break;
        default:
            break;
    }
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
                if (gameState == GameState::GameOver) {
                    gameState = GameState::Start;
                    audioManager->playSFX("menuBeep.wav");
                }
                break;
            case SDLK_b:
                if (gameState == GameState::Pause) {
                    initGame();
                    gameState = GameState::Start;
                    audioManager->playSFX("menuBeep.wav");
                }
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
                if (gameState == GameState::Ready) {
                    gameState = GameState::NextLevel;
                    audioManager->pauseMusic();
                    audioManager->playSFX("menuBeep.wav");
                    // init a game with a new level:
                    initGame();
                }

                break;
            case SDLK_p:
                if (gameState == GameState::Running) {
                    gameState = GameState::Pause;
                    audioManager->playSFX("menuBeep.wav");
                } else if (gameState == GameState::Pause) {
                    gameState = GameState::Running;
                    audioManager->playSFX("menuBeep.wav");
                }
                break;
            case SDLK_UP:
				if (canEnterNextLevel) {
					if (emeraldCounter >= Level::getEmeraldsNeeded() && player->getComponent<Player>()->exit) {
						canEnterNextLevel = false;
						if (emeraldCounter != 0) {
							audioManager->playMusic("finish.mp3", 4, 0);
						}
						gameState = GameState::NextLevel;
						completedLevel(currentLevel);
						currentStartPosition = nextStartPosition;
						currentLevel = nextLevel;
						initGame();
						emeraldCounter = 0;
					}
				}
                break;
            case SDLK_ESCAPE:
                if (gameState != GameState::Pause) {
                    audioManager->playSFX("menuBeep.wav");
                    initGame();
                    gameState = GameState::Start;
                }
                break;
            case SDLK_LCTRL:
                player->getComponent<Player>()->fireCannon(obstaclesAtlas);
                break;
            default:
                break;
        }
    }
}

void EmeraldGame::update(float time) {

    // ================================ UPDATE GAME STATES =============================
    // ========================== by: Sergiy Isakov 19.11.18 00:52 =====================
    if (gameState == GameState::Running) {
        updatePhysics();
        if (time > 0.03) // if framerate approx 30 fps then run two physics steps
            updatePhysics();

        for (auto &gameObject : gameObjectsList) {
            gameObject->update(time);
        }

        if (player->getComponent<Player>()->lostLife) {
            livesCounter--;
            player->getComponent<Player>()->lostLife = false;

            if (player->getComponent<Player>()->inPit) {
                player->getComponent<Player>()->inPit = false;
                runGame();
            }
        }
        if (livesCounter < 1) {
            initGame();
            currentLevel = 0;
            gameState = GameState::GameOver;
            audioManager->playSFX("gameOver.mp3", 4);
        }
        // ==========================    CHANGES GRAVITY    =============================
        // ========================== by: Eimantas Urbutis  =============================
        if (currentLevel == 2) {
            float gravity = -9.8;
            if (player->position.x > EmeraldGame::gameInstance->getLevel()->getWidth() / 2 &&
                player->position.y > EmeraldGame::gameInstance->getLevel()->getHeight() / 10) {
                gravity = (300 / sqrt(player->position.y));
                world = EmeraldGame::gameInstance->world;
                world->SetGravity(b2Vec2(0.0f, gravity));
            } else {
                gravity = (-150 / sqrt(player->position.y));
                world = EmeraldGame::gameInstance->world;
                world->SetGravity(b2Vec2(0.0f, gravity));
            }
            for (auto &go : gameObjectsList) {
                if (go->getPosition().x > EmeraldGame::gameInstance->getLevel()->getWidth() / 2 &&
                    go->position.y > EmeraldGame::gameInstance->getLevel()->getHeight() / 10) {
                    if (go->getComponent<SpriteComponent>()) {
                        if (go->name != "Player" && go->name != "Enemy" &&
                            go->getPosition().y > EmeraldGame::gameInstance->getLevel()->getHeight() / 10) {
                            auto currentSprite = go->getComponent<SpriteComponent>()->getSprite();
                            currentSprite.setFlip({false, true});
                            go->getComponent<SpriteComponent>()->setSprite(currentSprite);
                        }
                    }
                }
            }
        }
    } else if (gameState == GameState::NextLevel) {
        // animate a next level screen for some time:
        nextLevelDelta += time;
        if (nextLevelDelta >= 2.0f) {
            level->clearEmeralds();
            nextLevelDelta = 0;
			canEnterNextLevel = true;
            runGame();
        }
    } else if (gameState == GameState::Start) {
        audioManager->playMusic("bgmusic.mp3", 4);
        gameState = GameState::Ready;
        background.initStaticBackground("start.png");
        level->clearEmeralds();
		initVariables();
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

    // ================================ RENDER GAME STATES ============================
    // ======================= by: Sergiy Isakov 02 - 03.11.18 ========================
    if (gameState == GameState::GameOver) {
        spriteBatchBuilder.addSprite(gameOverSprite);
    } else if (gameState == GameState::Pause) {
        pauseSprite.setPosition(pos);
        spriteBatchBuilder.addSprite(pauseSprite);
    } else if (gameState == GameState::NextLevel) {
        spriteBatchBuilder.addSprite(levelSprite);
        auto levelNumber = uiAtlas->get("numeral" + to_string(currentLevel) + ".png");
        levelNumber.setScale({2.0f, 2.0f});
        levelNumber.setPosition({100.0f, 5.0f});
        spriteBatchBuilder.addSprite(levelNumber);
    }

    // ======================== UI LIVES AND EMERALDS ==================================
    // ===================== by: Sergiy Isakov 17.11.18 22.17 =======================
    if (gameState == GameState::Running || gameState == GameState::Pause) {
        auto livesSprite = gameSpritesAtlas->get("spr_heart" + to_string(livesCounter) + ".png");
        livesSprite.setPosition({pos.x - windowSize.x * 0.4f, pos.y + windowSize.y * 0.45f});
        livesSprite.setScale(EmeraldGame::scale * 1.5f);
        spriteBatchBuilder.addSprite(livesSprite);

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

void EmeraldGame::addGameObject(shared_ptr<GameObject> obj) {
    gameObjectsList.push_back(obj);
}

void EmeraldGame::addGameObjectsVector(vector<shared_ptr<GameObject>> objects) {
    gameObjectsList.insert(gameObjectsList.end(), objects.begin(), objects.end());
}

void EmeraldGame::deleteGameObject(shared_ptr<GameObject> gameObject) {
    gameObjectsList.erase(remove(gameObjectsList.begin(), gameObjectsList.end(), gameObject), gameObjectsList.end());
}

void EmeraldGame::deleteGameObjectsByName(string name) {
    for (auto &go : gameObjectsList) {
        if (go->name == name) {
            deleteGameObject(go);
        }
    }
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

int EmeraldGame::getEmeraldCounter() {
    return emeraldCounter;
}

shared_ptr<SpriteAtlas> EmeraldGame::getGameSpriteAtlas() {
    return gameSpritesAtlas;
}


