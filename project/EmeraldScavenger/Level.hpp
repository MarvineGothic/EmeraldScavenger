#pragma once

#include <memory>
#include <vector>
#include <sre/SpriteAtlas.hpp>

using namespace std;
using namespace sre;
using namespace glm;

class EmeraldGame;

class GameObject;

class PlatformComponent;

enum class TileCollider {
    Block,
    BlockHalf,
    Incline,
    Decline
};

class Level {
    Level() = default;

    float levelWidth;
    float levelHeight;
    EmeraldGame *game;
    shared_ptr<SpriteAtlas> spriteAtlas;
public:
    static shared_ptr<Level> createDefaultLevel(EmeraldGame *game, shared_ptr<SpriteAtlas> spriteAtlas);

    void level_01();

    void level_02();

    void level_03();

    void level_04();

    void level_05();

    shared_ptr<PlatformComponent> addPlatform(int x, int y, int startSpriteId, int length, bool kinematic);

    shared_ptr<PlatformComponent> addWall(int x, int y, int startSpriteId, int height);

    static constexpr float tileSize = 21;

    void generateLevel();

    void Level::Procedural_level();

    vec2 min;
    vec2 max;

    float Level::getWidth();

    float Level::getHeight();

};

