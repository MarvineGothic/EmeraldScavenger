#include <utility>

//
// Created by Morten Nobel-Jørgensen on 11/3/17.
//

#include "Level.hpp"
#include "EmeraldGame.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "PlatformComponent.hpp"
#include "MovingPlatformComponent.hpp"
#include "Enemy.hpp"


std::shared_ptr<Level> Level::createDefaultLevel(EmeraldGame *game,
                                                 std::shared_ptr<sre::SpriteAtlas> spriteAtlas,
                                                 std::shared_ptr<sre::SpriteAtlas> enemiesAtlas) {
    std::shared_ptr<Level> res = std::shared_ptr<Level>(new Level());
    res->game = game;
    res->spriteAtlas = std::move(spriteAtlas);
    res->enemiesAtlas = std::move(enemiesAtlas);

    return res;
}

void Level::makeLevel(int level) {
    switch (level) {
        case 0:
            this->level_00();
            break;
        case 1:
            this->level_01();
            break;
        case 2:
            this->level_02();
            break;
        case 3:
            this->level_03();
            break;
        case 4:
            this->level_04();
            break;
        case 5:
            this->level_05();
            break;
        default:
            this->generateLevel();
            break;
    }
}

void Level::level_00() {

    int width = 205;
    int height = 51;

    startPosition = {vec2{1.5, 2.5} * tileSize};
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png");

    // start wall
    addWall(0, 0, 2, height);
    // floor
    addPlatform(1, 0, 2, width - 200, false);
    // gap = 5
    addPlatform(10, 0, 2, width - 115, false);
    // gap = 5
    addPlatform(105, 0, 2, width - 105, false);
    // ceil
    addPlatform(1, height - 1, 2, width - 1, false);


    auto movingPlatform = addPlatform(10, 3, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({10, 3});
    movingPlatformComponent->setMovementEnd({10, 10});

    // add some enemies to the level
    addEnemy({50, 2}, Enemy::EnemyType::Zombie);
    addEnemy({25, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 2}, Enemy::EnemyType::Zombie);
    addEnemy({180, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 10}, Enemy::EnemyType::AngryBird);
    addEnemy({120, 5}, Enemy::EnemyType::Dragon);
    addEnemy({30, 5}, Enemy::EnemyType::AngryBird);
    addEnemy({50, 5}, Enemy::EnemyType::Dragon);

    // add some emeralds:
    // todo: stop rendering collected emeralds
    //create a list of positions for emeralds and safe them to emeraldGame
    // when emerald is taken, position is deleted from the list
    addEmerald({15, 1.5f});
    addEmerald({25, 1.5f});
    addEmerald({35, 1.5f});
    addEmerald({45, 1.5f});
    addEmerald({55, 1.5f});
    addEmerald({65, 1.5f});
    addEmerald({75, 1.5f});
    addEmerald({85, 1.5f});
    addEmerald({95, 1.5f});
    addEmerald({105, 1.5f});
    addEmerald({115, 1.5f});
    addEmerald({125, 1.5f});


    // add some more platforms
    addPlatform(15, 7, 2, 5, true);
    addPlatform(20, 3, 2, 5, true);
    addPlatform(25, 7, 2, 5, true);
    addPlatform(30, 10, 2, 5, true);
    addPlatform(35, 7, 2, 5, true);
    addPlatform(40, 3, 2, 5, true);

    addPlatform(35, 15, 2, 5, true);
    addPlatform(40, 20, 2, 5, true);
    addPlatform(45, 25, 2, 5, true);
    addPlatform(50, 30, 2, 5, true);
    addPlatform(55, 35, 2, 5, true);
    addPlatform(60, 40, 2, 5, true);
    addPlatform(65, 45, 2, 140, true);


    addWall(125, 0, 2, 2);
    // end wall
    addWall(width, 0, 2, height);

}

//todo: design levels:

void Level::level_01() {

}

void Level::level_02() {

}

void Level::level_03() {

}

void Level::level_04() {

}

void Level::level_05() {

}

void Level::generateLevel() {

    auto movingPlatform = addPlatform(10, 3, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatform->getPhysicsComponent()->setLinearVelocity({0, 100});
    b2Body *body = movingPlatform->getPhysicsComponent()->getBody();
    movingPlatformComponent->setBody(body);
    movingPlatformComponent->setMovementStart({10, 3});
    movingPlatformComponent->setMovementEnd({10, 5});

    //Procedurally generated level
    Procedural_level();
}

//clamps a value with a lower and upper bound
float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

//Procedurally generates a level.
void Level::Procedural_level() {

    int width = 500;
    int height = 50;

    // start wall
    addWall(-1, 0, 2, height);
    // floor
    addPlatform(0, 0, 2, 10, false);
    // ceil
    addPlatform(0, height, 2, width, false);

    startPosition = {vec2{1.5, 2.5} * Level::tileSize};
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png");

    glm::vec2 min(0, -3);
    glm::vec2 max(5, 3);

    float floor = 1;
    float ceil = 15;
    float wall_l = 0;
    float wall_r = 500;


    addPlatform(15, 7, 2, 5, true);
    glm::vec2 prev_platform = glm::vec2(15 + 5, 7);

    int i = 0;
    while (i < 100) {
        //int min_y = clamp(min.y, floor, ceil);
        //int max_y = clamp(max.y, floor, ceil);

        int length = (rand() % 5) + 1;;
        int rand_x = static_cast<int>((rand() % (int) ((max.x - min.x) + 1)) + min.x + prev_platform.x);
        int rand_y = clamp(static_cast<int>((rand() % (int) ((max.y - min.y) + 1)) + min.y + prev_platform.y), floor,
                           ceil);
        addPlatform(rand_x, rand_y, 2, length, true);
        prev_platform = glm::vec2(rand_x + length, rand_y);
        i++;
    }
}

std::shared_ptr<PlatformComponent> Level::addPlatform(int x, int y, int startSpriteId, int length, bool kinematic) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initPlatform(spriteAtlas, x, y, startSpriteId, length, kinematic);
    return res;
}


std::shared_ptr<PlatformComponent> Level::addWall(int x, int y, int startSpriteId, int length) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Wall";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initWall(spriteAtlas, x, y, startSpriteId, length);
    return res;
}

std::shared_ptr<Enemy> Level::addEnemy(vec2 pos, Enemy::EnemyType enemyType) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Enemy";
    auto res = gameObject->addComponent<Enemy>();
    res->initEnemy(enemiesAtlas, pos, enemyType);
    return res;
}

shared_ptr<GameObject> Level::addEmerald(vec2 position) {
    auto emeraldObject = game->createGameObject();
    emeraldObject->name = "Emerald";
    auto emeraldSpriteComponent = emeraldObject->addComponent<SpriteComponent>();
    auto emeraldPhysicsComponent = emeraldObject->addComponent<PhysicsComponent>();
    auto emeraldSprite = spriteAtlas->get("diamond blue.png");
    emeraldSprite.setScale(EmeraldGame::scale/2.0f);
    emeraldSpriteComponent->setSprite(emeraldSprite);

    emeraldObject->setPosition(position * tileSize);

    auto physicsScale = EmeraldGame::gameInstance->physicsScale;
    emeraldPhysicsComponent->initCircle(b2_staticBody,
                                        emeraldSprite.getSpriteSize().x / (physicsScale * tileSize),
                                        position * tileSize / physicsScale,
                                        1);
    emeraldPhysicsComponent->setSensor(true);
    return emeraldObject;
}

int Level::getWidth() {
    return this->levelWidth;
}

int Level::getHeight() {
    return this->levelHeight;
}

vec2 Level::getStartPos() {
    return this->startPosition;
}

vec2 Level::getFinishPos() {
    return this->finishPosition;
}


