#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "SDL_mixer.h"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
#include "SideScrollingCamera.hpp"
#include "Box2DDebugDraw.hpp"
#include "Level.hpp"
#include "BirdMovementComponent.hpp"
#include "Background.hpp"
#include "AudioManager.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class PhysicsComponent;

enum class GameState {
    Start,
    Pause,
    Ready,
    Running,
    GameOver,
    NextLevel
};

class EmeraldGame : public b2ContactListener {
    const float physicsScale = 100;
    bool doDebugDraw = false;

    int livesCounter = 5;
    int emeraldCounter = 0;
    float nextLevelDelta = 0.0f;

    SDLRenderer r;
    Box2DDebugDraw debugDraw;
    GameState gameState = GameState::Start;

    shared_ptr<GameObject> player;
    shared_ptr<SideScrollingCamera> camera;
    shared_ptr<SpriteAtlas> obstaclesAtlas;
    shared_ptr<SpriteAtlas> gameSpritesAtlas;
    shared_ptr<SpriteAtlas> uiAtlas;
	shared_ptr<SpriteAtlas> platformerArtAtlas;
    shared_ptr<Level> level;

    Sprite gameOverSprite, pauseSprite, levelSprite, emeraldSprite;

    vector<shared_ptr<GameObject>> gameObjectsList;
    map<b2Fixture *, PhysicsComponent *> physicsComponentMap;

    b2World *world = nullptr;

    AudioManager *audioManager;

    void initAssets();

    void initGame();

    void runGame();

    void resetGame();

    void initCamera();

    void initPhysics();

    void initLevel();

    void initPlayer();

	void initVariables();

    void update(float time);

    void render();

	void completedLevel(Level::LevelName level);

    void onKey(SDL_Event &event);

    void handleContact(b2Contact *contact, bool begin);

    void updatePhysics();

    void registerPhysicsComponent(PhysicsComponent *r);

    void deregisterPhysicsComponent(PhysicsComponent *r);

    void setGameState(GameState newState);

    friend class PhysicsComponent;

    friend class Level;

    friend class Entity;

    friend class Player;

    friend class Enemy;

    friend class Door;

    friend class CollectibleItem;

    friend class PlatformComponent;

    friend class SideScrollingCamera;

public:
    EmeraldGame();

    static Level::LevelName currentLevel, nextLevel;
	static vec2 currentStartPosition, nextStartPosition;

    shared_ptr<GameObject> createGameObject();

    void addGameObject(shared_ptr<GameObject> obj);

    void addGameObjectsVector(vector<shared_ptr<GameObject>> objects);

    void deleteGameObject(shared_ptr<GameObject> gameObject);
    
    void deleteGameObjectsByName(string name);

    static const vec2 windowSize;
    static const vec2 scale;

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;

    Background background;
    static EmeraldGame *gameInstance;

    static constexpr float32 timeStep = 1.0f / 60.0f;

    shared_ptr<Level> getLevel();

    int getEmeraldCounter();

    shared_ptr<SpriteAtlas> getGameSpriteAtlas();

    time_t rand_seed;

	static bool introCleared, physCleared, gravCleared, procCleared, bonusCleared;

};
