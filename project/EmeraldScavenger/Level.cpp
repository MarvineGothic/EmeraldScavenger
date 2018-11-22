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
#include "Door.hpp"
#include "CollectibleItem.hpp"

vec2 Level::startPosition = {};
vec2 Level::finishPosition = {};

shared_ptr<Level> Level::createDefaultLevel(EmeraldGame *game) {
    shared_ptr<Level> res = shared_ptr<Level>(new Level());
    res->game = game;
    return res;
}

//clamps a value with a lower and upper bound
float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

void Level::makeLevel(int level) {
    switch (level) {
        case 0:
            this->level_00();
            break;
        case 1:
            this->level_01();
            break;
        default:
            this->generateLevel();
            break;
    }
}

void Level::level_00() {

    int width = 205;
    int height = 51;

    startPosition = vec2{2.5, 2.4};
    finishPosition = vec2{width - 2.5f, 2.4f};
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

    // doors:
    addDoor(startPosition, true, false);
    addDoor(finishPosition, false, true);
    // add some enemies to the level
    addEnemy({50, 2}, Enemy::EnemyType::Zombie);
    addEnemy({25, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 2}, Enemy::EnemyType::Zombie);
    addEnemy({180, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 10}, Enemy::EnemyType::AngryBird);
    addEnemy({120, 5}, Enemy::EnemyType::Dragon);
    addEnemy({30, 5}, Enemy::EnemyType::AngryBird);
    addEnemy({50, 5}, Enemy::EnemyType::Dragon);

    // add some collectibles:
    // stop rendering collected collectibles
    //create a list of positions for collectibles and safe them to emeraldGame
    // when emerald is taken, position is deleted from the list
    // don't forget to clean collectibles vector when level is finished

    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty()) {
        collectibles.emplace_back(addCollectible(vec2(15, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(25, 1.5f), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(95, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(105, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(115, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(200, 1.5f), "Emerald"));
    } else {
        for (auto &cItem : collectibles) {
            temp.emplace_back(addCollectible(cItem->getPosition(), cItem->getName()));
        }
        collectibles.clear();
        for (auto &cItem : temp) {
            collectibles.emplace_back(cItem);
        }
        temp.clear();
    }


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
    int width = 205;
    int height = 51;

    startPosition = vec2{200.0, height - 3.5};
    finishPosition = vec2{width - 2.5f, 2.4f};
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

    // doors:
    addDoor(startPosition, true, false);
    addDoor(finishPosition, false, true);

    auto movingPlatform = addPlatform(10, 3, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({10, 3});
    movingPlatformComponent->setMovementEnd({10, 10});

    // add some enemies to the level
    /*addEnemy({50, 2}, Enemy::EnemyType::Zombie);
    addEnemy({25, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 2}, Enemy::EnemyType::Zombie);
    addEnemy({180, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 10}, Enemy::EnemyType::AngryBird);
    addEnemy({120, 5}, Enemy::EnemyType::Dragon);
    addEnemy({30, 5}, Enemy::EnemyType::AngryBird);
    addEnemy({50, 5}, Enemy::EnemyType::Dragon);*/

    // add some collectibles:
    // stop rendering collected collectibles
    //create a list of positions for collectibles and safe them to emeraldGame
    // when emerald is taken, position is deleted from the list
    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty()) {
        collectibles.emplace_back(addCollectible(vec2(25, 1.5f), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(75, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(85, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(95, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(105, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(115, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(200, 1.5f), "Emerald"));
    }else {
        for (auto &cItem : collectibles) {
            temp.emplace_back(addCollectible(cItem->getPosition(), cItem->getName()));
        }
        collectibles.clear();
        for (auto &cItem : temp) {
            collectibles.emplace_back(cItem);
        }
        temp.clear();
    }


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

    startPosition = vec2{1.5, 2.5};
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png");

    // add some collectibles:
    // stop rendering collected collectibles
    //create a list of positions for collectibles and safe them to emeraldGame
    // when emerald is taken, position is deleted from the list
    /*if (collectibles.empty()) {
        collectibles[vec2(85, 1.5f)] = "Emerald";
        collectibles[vec2(95, 1.5f)] = "Emerald";
        collectibles[vec2(105, 1.5f)] = "Emerald";
        collectibles[vec2(115, 1.5f)] = "Emerald";
        collectibles[vec2(125, 1.5f)] = "Emerald";
    }
    // just add existing collectibles to EmeraldGame gameObjectsList
    for (auto &emeraldPos : collectibles) {
        addCollectible(emeraldPos.first, emeraldPos.second);
    }*/

    levelMin = {0, -3};
    levelMax = {5, 3};

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
        int rand_x = static_cast<int>((rand() % (int) ((levelMax.x - levelMin.x) + 1)) + levelMin.x + prev_platform.x);
        int rand_y = clamp(
                static_cast<int>((rand() % (int) ((levelMax.y - levelMin.y) + 1)) + levelMin.y + prev_platform.y),
                floor,
                ceil);
        addPlatform(rand_x, rand_y, 2, length, true);
        prev_platform = glm::vec2(rand_x + length, rand_y);
        i++;
    }
}

shared_ptr<PlatformComponent> Level::addPlatform(int x, int y, int startSpriteId, int length, bool kinematic) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initPlatform(EmeraldGame::gameInstance->obstaclesAtlas, x, y, startSpriteId, length, kinematic);
    return res;
}


shared_ptr<PlatformComponent> Level::addWall(int x, int y, int startSpriteId, int length) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Wall";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initWall(EmeraldGame::gameInstance->obstaclesAtlas, x, y, startSpriteId, length);
    return res;
}


shared_ptr<Enemy> Level::addEnemy(vec2 pos, Enemy::EnemyType enemyType) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Enemy";
    auto res = gameObject->addComponent<Enemy>();
    res->initEnemy(EmeraldGame::gameInstance->enemiesAtlas, pos, enemyType);
    return res;
}

shared_ptr<Door> Level::addDoor(vec2 position, bool isOpen, bool isExit) {
    auto doorObject = game->createGameObject();
    doorObject->name = "Door";
    auto door = doorObject->addComponent<Door>();
    door->initDoor(position, isOpen, isExit);
    return door;
}

shared_ptr<CollectibleItem> Level::addCollectible(vec2 position, string name) {
    auto collectibleObject = game->createGameObject();
    collectibleObject->name = name;
    auto collectible = collectibleObject->addComponent<CollectibleItem>();
    collectible->initCollectible(position, name);
    collectible->gameObjectCopy = collectibleObject;
    return collectible;
}

int Level::getWidth() {
    return this->levelWidth;
}

int Level::getHeight() {
    return this->levelHeight;
}

vec2 Level::getFinishPos() {
    return finishPosition * tileSize;
}

vec2 Level::getStartPos() {
    return startPosition * tileSize;
}

void Level::clearEmeralds() {
    this->collectibles.clear();
}

void Level::deleteEmerald(shared_ptr<CollectibleItem> item) {
    collectibles.erase(remove(collectibles.begin(), collectibles.end(), item), collectibles.end());
    /*auto comp = find(collectibles.begin(), collectibles.end(), item);
    if (comp != collectibles.end()) {
        collectibles.erase(comp);
    }*/
}

