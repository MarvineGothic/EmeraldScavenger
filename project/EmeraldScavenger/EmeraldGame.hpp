#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
#include "SideScrollingCamera.hpp"
#include "Box2DDebugDraw.hpp"
#include "Level.hpp"
#include "BirdMovementComponent.hpp"
#include "Background.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class PhysicsComponent;

enum class GameState {
    Start,
    GetReady,
    Ready,
    Running,
    GameOver
};

class EmeraldGame : public b2ContactListener {
    const float physicsScale = 100;
    bool doDebugDraw = false;

    int levelCounter = 0;
    int livesCounter = 3;

    SDLRenderer r;
    Box2DDebugDraw debugDraw;
    GameState gameState = GameState::Start;

    shared_ptr<GameObject> player;
    shared_ptr<SideScrollingCamera> camera;
    shared_ptr<SpriteAtlas> spriteAtlas;
    shared_ptr<Level> level;

    vector<shared_ptr<GameObject>> gameObjectsList;
    map<b2Fixture *, PhysicsComponent *> physicsComponentMap;

    b2World *world = nullptr;

    void initGame();

    void runGame();

    void resetGame();

    void initCamera();

    void initPhysics();

    void initLevel();

    void initPlayer(vec2 position);


    void update(float time);

    void render();

    void onKey(SDL_Event &event);

    void handleContact(b2Contact *contact, bool begin);

    void updatePhysics();

    void registerPhysicsComponent(PhysicsComponent *r);

    void deregisterPhysicsComponent(PhysicsComponent *r);

    void setGameState(GameState newState);

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

    Background background;
    static EmeraldGame *gameInstance;

    static constexpr float32 timeStep = 1.0f / 60.0f;

    shared_ptr<Level> getLevel();

};
