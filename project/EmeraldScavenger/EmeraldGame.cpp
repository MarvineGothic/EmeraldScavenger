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

const vec2 EmeraldGame::windowSize(800, 600);
EmeraldGame *EmeraldGame::gameInstance = nullptr;

EmeraldGame::EmeraldGame()
        : debugDraw(physicsScale) {
    gameInstance = this;
    r.setWindowSize(windowSize);
    bool useVsync = true;
    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL)
            .withVSync(useVsync);

    backgroundColor = {0.6f, 0.6f, 1.0f, 1.0f};

    //spriteAtlas = SpriteAtlas::create("platformer-art-deluxe.json","platformer-art-deluxe.png");
    spriteAtlas = SpriteAtlas::create("platformer-art-deluxe.json", Texture::create()
            .withFile("platformer-art-deluxe.png")
            .withFilterSampling(false)
            .build());

    level = Level::createDefaultLevel(this, spriteAtlas);

    initLevel();

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

void EmeraldGame::initLevel() {
    initPhysics();

    auto player = createGameObject();
    player->name = "Player";
    auto playerSprite = player->addComponent<SpriteComponent>();
    auto playerSpriteObj = spriteAtlas->get("19.png");
    playerSpriteObj.setPosition(vec2{1.5, 2.5} * Level::tileSize);
    playerSprite->setSprite(playerSpriteObj);
    auto characterController = player->addComponent<Player>();
    characterController->setSprites(
            spriteAtlas->get("19.png"),
            spriteAtlas->get("20.png"),
            spriteAtlas->get("21.png"),
            spriteAtlas->get("26.png"),
            spriteAtlas->get("27.png"),
            spriteAtlas->get("28.png")
    );

    auto camObj = createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    //camObj->setPosition(windowSize );
    camera->setFollowObject(player, {windowSize * 0.5f});


    auto birdObj = createGameObject();
    birdObj->name = "Bird";
    auto spriteComponent = birdObj->addComponent<SpriteComponent>();
    auto bird = spriteAtlas->get("433.png");
    bird.setFlip({true, false});
    spriteComponent->setSprite(bird);
    birdMovement = birdObj->addComponent<BirdMovementComponent>().get();

    birdMovement->setPositions({
                                       {-50,  350},
                                       {0,    300},
                                       {50,   350},
                                       {100,  400},
                                       {150,  300},
                                       {200,  200},
                                       {250,  300},
                                       {300,  400},
                                       {350,  350},
                                       {400,  300},
                                       {450,  350},
                                       {500,  400},
                                       {550,  350},
                                       {600,  300},
                                       {650,  350},
                                       {700,  400},
                                       {750,  350},
                                       {800,  300},
                                       {850,  350},
                                       {900,  400},
                                       {950,  350},
                                       {1000, 300},
                                       {1050, 350},
                                       {1100, 400},
                                       {1150, 350},
                                       {1200, 300},
                                       {1250, 350},
                               });

    //level->level_01();
    level->generateLevel();
}

void EmeraldGame::update(float time) {
    updatePhysics();
    if (time > 0.03) // if framerate approx 30 fps then run two physics steps
    {
        updatePhysics();
    }
    for (auto &sceneObject : sceneObjects) {
        sceneObject->update(time);
    }
}

void EmeraldGame::render() {
    auto rp = RenderPass::create()
            .withCamera(camera->getCamera())
            .build();

    if (doDebugDraw) {
        static Inspector profiler;
        profiler.update();
        profiler.gui(false);

        vector<vec3> lines;
        for (int i = 0; i < 5000; i++) {
            float t = (i / 5001.0f) * birdMovement->getNumberOfSegments();
            float t1 = ((i + 1) / 5001.0f) * birdMovement->getNumberOfSegments();
            auto p = birdMovement->computePositionAtTime(t);
            auto p1 = birdMovement->computePositionAtTime(t1);
            lines.emplace_back(p, 0);
            lines.emplace_back(p1, 0);
        }
        rp.drawLines(lines);
    }

    auto pos = camera->getGameObject()->getPosition();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto &go : sceneObjects) {
        go->renderSprite(spriteBatchBuilder);
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    if (doDebugDraw) {
        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();
    }
}

void EmeraldGame::onKey(SDL_Event &event) {
    for (auto &gameObject: sceneObjects) {
        for (auto &c : gameObject->getComponents()) {
            bool consumed = c->onKey(event);
            if (consumed) {
                return;
            }
        }
    }

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
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
            default:
                break;
        }
    }
}

shared_ptr<GameObject> EmeraldGame::createGameObject() {
    auto obj = shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}

void EmeraldGame::updatePhysics() {

    const int positionIterations = 4;
    const int velocityIterations = 12;
    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto phys : physicsComponentLookup) {
        PhysicsComponent *physicsComponent = phys.second;
        if (!physicsComponent->isAutoUpdate()) continue;
        auto position = physicsComponent->getBody()->GetPosition();
        float angle = physicsComponent->getBody()->GetAngle();
        auto gameObject = physicsComponent->getGameObject();
        gameObject->setPosition(vec2(position.x * physicsScale, position.y * physicsScale));
        gameObject->setRotation(angle);
    }
}

void EmeraldGame::initPhysics() {
    float gravity = -9.8f; // 9.8 m/s2
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);

    if (doDebugDraw) {
        world->SetDebugDraw(&debugDraw);
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
    auto iter = physicsComponentLookup.find(r->getFixture());
    if (iter != physicsComponentLookup.end()) {
        physicsComponentLookup.erase(iter);
    } else {
        assert(false); // cannot find physics object
    }
}

void EmeraldGame::registerPhysicsComponent(PhysicsComponent *r) {
    physicsComponentLookup[r->getFixture()] = r;
}

void EmeraldGame::handleContact(b2Contact *contact, bool begin) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if (physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end()) {
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
