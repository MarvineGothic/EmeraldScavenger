#pragma once

#include <memory>
#include <vector>
#include <sre/SpriteAtlas.hpp>
#include "Enemy.hpp"

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

    int levelWidth, levelHeight;
    vec2 startPosition, finishPosition;
    EmeraldGame *game;
    shared_ptr<SpriteAtlas> spriteAtlas;
    shared_ptr<SpriteAtlas> enemiesAtlas;
public:
    static shared_ptr<Level> createDefaultLevel(EmeraldGame *game,
                                                shared_ptr<SpriteAtlas> spriteAtlas,
                                                shared_ptr<SpriteAtlas> enemiesAtlas);

    void makeLevel(int level);

    void level_00();

    void level_01();

    void level_02();

    void level_03();

    void level_04();

    void level_05();

    shared_ptr<PlatformComponent> addPlatform(int x, int y, int startSpriteId, int length, bool kinematic);

    shared_ptr<PlatformComponent> addWall(int x, int y, int startSpriteId, int height);

    shared_ptr<Enemy> addEnemy(vec2 position, Enemy::EnemyType enemyType);
    
    static constexpr float tileSize = 21;

    void generateLevel();

    void Procedural_level();

    vec2 min;
    vec2 max;

    int getWidth();

    int getHeight();

    vec2 getStartPos();

    vec2 getFinishPos();
};
