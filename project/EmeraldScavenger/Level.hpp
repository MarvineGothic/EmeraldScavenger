#pragma once

#include <memory>
#include <vector>
#include <sre/SpriteAtlas.hpp>
#include "Enemy.hpp"
#include "Door.hpp"
#include "CollectibleItem.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class EmeraldGame;

class GameObject;

class PlatformComponent;

class Enemy;

enum class TileCollider {
    Block,
    BlockHalf,
    Incline,
    Decline
};

class Level {
    Level() = default;

    int levelWidth{}, levelHeight{};

    vec2 levelMin{}, levelMax{};
    static vec2 startPosition, finishPosition;
    EmeraldGame *game{};

    vector<shared_ptr<CollectibleItem>> collectibles;

public:
    static shared_ptr<Level> createDefaultLevel(EmeraldGame *game);

    void makeLevel(int level);

    void level_intro();

    void level_hub();

    void level_phys();

    void level_grav();

    void level_proc();

    void level_test();

	void level_bonus_1();

	void level_bonus_2();

    shared_ptr<PlatformComponent> addPlatform(int x, int y, int startSpriteId, int length, bool kinematic);

    shared_ptr<PlatformComponent> addWall(int x, int y, int startSpriteId, int height);

    shared_ptr<Enemy> addEnemy(vec2 position, Enemy::EnemyType enemyType);

    shared_ptr<CollectibleItem> addCollectible(vec2 position, string name);

    shared_ptr<Door> addDoor(vec2 position, bool isOpen, bool isExit);

    static constexpr float tileSize = 21;

    void clearEmeralds();

    void deleteEmerald(shared_ptr<CollectibleItem> item);

    void generateLevel();

    void Procedural_level();

    int getWidth();

    int getHeight();

    static vec2 getStartPos();

    static vec2 getFinishPos();
};