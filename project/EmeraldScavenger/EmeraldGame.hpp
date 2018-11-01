#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
#include "SideScrollingCamera.hpp"
#include "Box2DDebugDraw.hpp"
#include "Level.hpp"
#include "BirdMovementComponent.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class PhysicsComponent;

enum class GameState{
    Ready,
    Running,
    GameOver
};

class EmeraldGame : public b2ContactListener {
    SDLRenderer r;

    void initLevel();
    void initPhysics();

    void update(float time);

    void render();

    void onKey(SDL_Event &event);

    void handleContact(b2Contact *contact, bool begin);

    shared_ptr<SideScrollingCamera> camera;
    shared_ptr<SpriteAtlas> spriteAtlas;

    vector<shared_ptr<GameObject>> sceneObjects;

    void updatePhysics();

    shared_ptr<Level> level;

    // todo: add some textures to background
    Color backgroundColor;
    b2World * world = nullptr;
    BirdMovementComponent* birdMovement;
    const float physicsScale = 100;
    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    map<b2Fixture*,PhysicsComponent *> physicsComponentLookup;
    Box2DDebugDraw debugDraw;
    bool doDebugDraw = false;
    friend class PhysicsComponent;
    friend class Level;
    friend class Player;
    friend class PlatformComponent;
    friend class SideScrollingCamera;
public:
    EmeraldGame();

    shared_ptr<GameObject> createGameObject();
    static const vec2 windowSize;

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;

    static EmeraldGame* gameInstance;

    static constexpr float32 timeStep = 1.0f / 60.0f;

    shared_ptr<Level> getLevel();

};

