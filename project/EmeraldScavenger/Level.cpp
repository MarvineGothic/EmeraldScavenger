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
    // platform sprites:
    res->ground = EmeraldGame::gameInstance->obstaclesAtlas->get("tile ground.png");
    res->brick = EmeraldGame::gameInstance->obstaclesAtlas->get("brick_1.png");
    res->moss = EmeraldGame::gameInstance->obstaclesAtlas->get("moss_tile.png");
    return res;
}

void Level::makeLevel(int level) {
    switch (level) {
        case 0:
            this->level_bonus_0();
            break;
        case 1:
            this->level_hub();
            break;
        case 2:
            this->level_grav();
            break;
        case 3:
            this->level_phys();
            break;
        case 4:
            this->level_proc();
            break;
        case 5:
            this->level_test();
            break;
        case 6:
            this->level_bonus_0();
            break;
        case 7:
            this->level_bonus_1();
            break;
        case 8:
            this->level_bonus_2();
            break;
        default:
            this->level_test();
            break;
    }
}

void Level::level_intro() {

}
//todo: design levels:
void Level::level_hub() {

    game->background.initDynamicBackground("background.png");

    int width = 25;
    int height = 30;

    startPosition = vec2{1.5, 2.5};

    vec2 levelMin = {width, 0};
    vec2 levelMax = {width, height};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(0, 5, ground, height - 10);
    // floor
    addPlatform(0, 0, ground, width, false);
    // ceil
    addPlatform(0, height, ground, width, false);
    // end wall
    addWall(width, 5, brick, height - 10);

    //Elevator 1
    auto movingPlatform = addPlatform(1, 5, ground, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({1, 5});
    movingPlatformComponent->setMovementEnd({1, height - 5});
    movingPlatformComponent->setSpeed(10);

    //Elevator 1
    auto movingPlatform2 = addPlatform(width - 5, 5, ground, 5, true);
    auto movingPlatformComponent2 = movingPlatform2->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent2->setMovementStart({width - 5, 5});
    movingPlatformComponent2->setMovementEnd({width - 5, height - 5});
    movingPlatformComponent2->setSpeed(10);

}

void Level::level_grav() {

    game->background.initDynamicBackground("background.png");

    int width = 40;
    int height = 200;

    startPosition = vec2{width - 1.5, 2.5};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(0, 0, brick, height);
    //end wall
    addWall(width, 5, brick, height - 5);
    //Middle wall
    addWall(glm::floor(width / 2), 15, brick, height - 15);
    // ceil
    addPlatform(0, height, ground, width, false);
    // floor
    addPlatform(0, 0, ground, width, false);

}

void Level::level_phys() {

    game->background.initDynamicBackground("background.png");

    int width = 50;
    int height = 50;

    startPosition = vec2{1.5, 2.5};

    vec2 levelMin = {width + width, height - 5};
    vec2 levelMax = {width, height};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(0, 5, brick, height - 5);
    //end wall
    addWall(width, 0, brick, height);
    // ceil
    addPlatform(0, height, ground, width, false);
    // floor
    addPlatform(0, 0, ground, width, false);
}

//clamps a value with a lower and upper bound
float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}


//Procedurally generates a level.
void Level::level_proc() {

    game->background.initDynamicBackground("background.png");

    int width = 200;
    int height = 30;

    startPosition = vec2{1.5, 2.5};

    vec2 levelMin = {width + width, 0};
    vec2 levelMax = {width, height};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // end wall
    addWall(width, 0, brick, height);
    // floor
    addPlatform(0, 0, ground, 10, false);
    // ceil
    addPlatform(0, height, ground, width, false);

    //Defines how far apart platforms are allowed to be
    glm::vec2 min(2, -5);
    glm::vec2 max(6, 5);

    int max_length;
    int min_length;

    //Defines how long platforms are allowed to be
    if (game->livesCounter == 5) {
        max_length = 4;
        min_length = 1;
    } else if (game->livesCounter < 5 && game->livesCounter > 2) {
        max_length = 5;
        min_length = 2;
    } else {
        max_length = 6;
        min_length = 3;
    }

    float floor = 1;
    float ceil = 15;
    float wall_l = 0;
    float wall_r = 200;

    //Initial platform that starts chain
    addPlatform(15, 5, ground, 5, false);
    glm::vec2 prev_platform = glm::vec2(15 + 5, 5);

    /*
    Todo: Add special cases so jumps can either be very tall (max 6 in height)
                                                or very long (max TBD), but not both
          Add enemies
          Add spikes
          Add jewel-pickup (goal)
          Add moving platform back to beginning of level once completed
    */

    int i = 0;
    while (prev_platform.x < (width - 30)) {
        //int min_y = clamp(min.y, floor, ceil);
        //int max_y = clamp(max.y, floor, ceil);

        int length = (rand() % max_length) + min_length;
        int rand_x = static_cast<int>((rand() % (int) ((max.x - min.x) + 1)) + min.x + prev_platform.x);
        int rand_y = clamp(static_cast<int>((rand() % (int) ((max.y - min.y) + 1)) + min.y + prev_platform.y), floor,
                           ceil);
        addPlatform(rand_x, rand_y, ground, length, false);
        prev_platform = glm::vec2(rand_x + length, rand_y);
    }
    addPlatform(width - 20, 0, ground, 20, false);
}

void Level::level_test() {

    game->background.initDynamicBackground("background.png");

    int width = 150;
    int height = 30;

    startPosition = vec2{10, 10};
    finishPosition = vec2{width - 2.5f, 2.4f};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    game->background.initDynamicBackground("background.png");

    //addPlatform(0, 0, 2, width - 0, false);

    // start wall
    addWall(0, 0, brick, height);

    // ceil
    addPlatform(0, height, ground, width, false);

    /*
    // floor
    addPlatform(1, 0, 2, 3, false);
    // gap = 5
    addPlatform(10, 0, 2, 86, false);
    // gap = 5
    addPlatform(101, 0, 2, width - (101), false);
    */

    addPlatform(0, 0, ground, width, false);

    //addPlatform(105, 0, 2, width  - 105, false);

    // ceil
    //addPlatform(1, height, 2, width - 1, false);

    auto movingPlatform = addPlatform(10, 3, ground, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({10, 3});
    movingPlatformComponent->setMovementEnd({10, 10});

    // add some enemies to the level
    addEnemy({50, 2}, Enemy::EnemyType::Zombie);
    addEnemy({25, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 2}, Enemy::EnemyType::Zombie);
    addEnemy({100, 2}, Enemy::EnemyType::Zombie);
    addEnemy({75, 10}, Enemy::EnemyType::AngryBird);
    addEnemy({100, 5}, Enemy::EnemyType::Dragon);
    addEnemy({30, 5}, Enemy::EnemyType::AngryBird);
    addEnemy({50, 5}, Enemy::EnemyType::Dragon);

    // add some more platforms
    addPlatform(15, 7, ground, 5, true);
    addPlatform(20, 3, ground, 5, true);
    addPlatform(25, 7, ground, 5, true);
    addPlatform(30, 10, ground, 5, true);
    addPlatform(35, 7, ground, 5, true);
    addPlatform(40, 3, ground, 5, true);

    /*
    addPlatform(35, 1015, ground, 5, true);
    addPlatform(40, 1020, ground, 5, true);
    addPlatform(45, 1025, ground, 5, true);
    addPlatform(50, 1030, ground, 5, true);
    addPlatform(55, 1035, ground, 5, true);
    addPlatform(60, 1040, ground, 5, true);
    addPlatform(65, 1045, ground, 140, true);
    */

    addWall(125, 0, brick, 2);
    // end wall
    //addWall(width, 0, 2, height);
}

void Level::level_bonus_0() {

    int width = 150;
    int height = 50;
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png");

    startPosition = vec2{2.5, 47.5};
    addDoor(startPosition, true, false);
    finishPosition = vec2{12.5, 47.5};
    addDoor(finishPosition, false, true);

    // start wall
    addWall(0, 0, brick, height); // 1w
    addWall(10, 40, brick, 20); // 2w
    addWall(10, 15, brick, 17); //3w
    addWall(10, 5, brick, 6);   //4w

    //
    addWall(35, 5, brick, 35); //5w
    //
    addWall(45, 35, brick, 10); //6w

    // ceil
    addPlatform(0, height, ground, width, false);

    // stairs
    addPlatform(1, 45, ground, 5, false);  //1
    addPlatform(5, 40, ground, 5, false); //2
    addPlatform(1, 35, ground, 5, false);  //3
    addPlatform(5, 30, ground, 5, false);  //4
    addPlatform(1, 25, ground, 5, false);  //5
    addPlatform(5, 20, ground, 5, false);  //6
    addPlatform(1, 15, ground, 5, false);  //7
    addPlatform(5, 10, ground, 5, false); //8
    addPlatform(1, 5, ground, 5, false);   //9

    addPlatform(11, 45, ground, 39, false);  //10
    addPlatform(11, 40, ground, 29, false);  //11
    addPlatform(11, 30, ground, 19, false);    //12
    addPlatform(40, 35, ground, 5, false);    //13
    addPlatform(36, 30, ground, width - 41, false);    //14
    addPlatform(11, 20 , ground, 5, false);    //15
    addPlatform(11, 5, ground, 29, false);    //16

    auto movingPlatform = addPlatform(45, 8, ground, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({45, 8});
    movingPlatformComponent->setMovementEnd({45, 25});
    movingPlatformComponent->setSpeed(3);

    addPlatform(50, 25, ground, width - 50, false);    //17
    addPlatform(50, 10, ground, 5, false);    //18

    movingPlatform = addPlatform(55, 10, ground, 5, true);
    movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({55, 10});
    movingPlatformComponent->setMovementEnd({80, 10});
    movingPlatformComponent->setSpeed(10);

    addPlatform(70, 5, ground, 5, false);    //19
    addPlatform(85, 10, ground, 5, false);    //20
    addPlatform(90, 5, ground, 5, false);    //21


    movingPlatform = addPlatform(105, 3, ground, 5, true);
    movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({105, 3});
    movingPlatformComponent->setMovementEnd({125, 3});
    movingPlatformComponent->setSpeed(10);

    addPlatform(50, 40, ground, 5, false);    //22
    addPlatform(55, 35, ground, 5, false);    //23

    // floor
    addPlatform(1, 0, ground, 50, false);
    addPlatform(95, 0, ground, 10, false);
    addPlatform(130, 0, ground, width-130, false);


    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty()) {
        collectibles.emplace_back(addCollectible(vec2(13, 42.5), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(13, 22.5), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(72.5, 7.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(135, 2.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(145, 2.5f), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(70, 32.5f), "Emerald"));
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
    // add Brick
    addBrick(vec2{13, 32.5}, EmeraldGame::gameInstance->gameSpritesAtlas->get("spr_stoneGrassTop.png"),
             EmeraldGame::scale * 5.0f, 0.8f, 1, 1);


    addEnemy({100, 2}, Enemy::EnemyType::Zombie);
    addEnemy({110, 10}, Enemy::EnemyType::Dragon);
    addEnemy({120, 2}, Enemy::EnemyType::AngryBird);

    //Add some spikes here (at x = 125)

    addWall(width, 0, brick, height); //End wall

    // add rock
    /*addRock(vec2{2.5, 1.5}, EmeraldGame::gameInstance->enemiesAtlas->get("frame-1_zombie_idle.png"),
            EmeraldGame::scale * 2.5f,
            0.2f, 0.5f, 1);*/
}

void Level::level_bonus_1() {

    int width = 205;
    int height = 51;

    startPosition = vec2{2.5, 2.4};
    finishPosition = vec2{width - 2.5f, 2.4f};
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png");

    // start wall
    addWall(0, 0, brick, height);
    // floor
    addPlatform(1, 0, ground, width - 200, false);
    // gap = 5
    addPlatform(10, 0, ground, width - 115, false);
    // gap = 5
    addPlatform(105, 0, ground, width - 105, false);
    // ceil
    addPlatform(1, height - 1, ground, width - 1, false);


    auto movingPlatform = addPlatform(10, 3, ground, 5, true);
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
    addPlatform(15, 7, ground, 5, true);
    addPlatform(20, 3, ground, 5, true);
    addPlatform(25, 7, ground, 5, true);
    addPlatform(30, 10, ground, 5, true);
    addPlatform(35, 7, ground, 5, true);
    addPlatform(40, 3, ground, 5, true);

    addPlatform(35, 15, ground, 5, true);
    addPlatform(40, 20, ground, 5, true);
    addPlatform(45, 25, ground, 5, true);
    addPlatform(50, 30, ground, 5, true);
    addPlatform(55, 35, ground, 5, true);
    addPlatform(60, 40, ground, 5, true);
    addPlatform(65, 45, ground, 140, true);


    addWall(125, 0, brick, 2);
    // end wall
    addWall(width, 0, brick, height);

}

//todo: design levels:

void Level::level_bonus_2() {
    int width = 205;
    int height = 51;

    startPosition = vec2{200.0, height - 3.5};
    finishPosition = vec2{width - 2.5f, 2.4f};
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png");

    // start wall
    addWall(0, 0, brick, height);
    // floor
    addPlatform(1, 0, ground, width - 200, false);
    // gap = 5
    addPlatform(10, 0, ground, width - 115, false);
    // gap = 5
    addPlatform(105, 0, ground, width - 105, false);
    // ceil
    addPlatform(1, height - 1, ground, width - 1, false);

    // doors:
    addDoor(startPosition, true, false);
    addDoor(finishPosition, false, true);

    auto movingPlatform = addPlatform(10, 3, ground, 5, true);
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
    addPlatform(15, 7, ground, 5, true);
    addPlatform(20, 3, ground, 5, true);
    addPlatform(25, 7, ground, 5, true);
    addPlatform(30, 10, ground, 5, true);
    addPlatform(35, 7, ground, 5, true);
    addPlatform(40, 3, ground, 5, true);

    addPlatform(35, 15, ground, 5, true);
    addPlatform(40, 20, ground, 5, true);
    addPlatform(45, 25, ground, 5, true);
    addPlatform(50, 30, ground, 5, true);
    addPlatform(55, 35, ground, 5, true);
    addPlatform(60, 40, ground, 5, true);
    addPlatform(65, 45, ground, 140, true);


    addWall(125, 0, brick, 2);
    // end wall
    addWall(width, 0, brick, height);
}

void Level::generateLevel() {
    auto movingPlatform = addPlatform(10, 3, ground, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatform->getPhysicsComponent()->setLinearVelocity({0, 100});
    b2Body *body = movingPlatform->getPhysicsComponent()->getBody();
    movingPlatformComponent->setBody(body);
    movingPlatformComponent->setMovementStart({10, 3});
    movingPlatformComponent->setMovementEnd({10, 5});

    //Procedurally generated level
    level_proc();
}

shared_ptr<PlatformComponent> Level::addPlatform(int x, int y, Sprite sprite, int length, bool kinematic) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initPlatform(x, y, sprite, length, kinematic);
    return res;
}


shared_ptr<PlatformComponent> Level::addWall(int x, int y, Sprite sprite, int length) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Wall";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initWall(x, y, sprite, length);
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

shared_ptr<Rock> Level::addRock(vec2 position, Sprite sprite, vec2 scale,
                                float restitution,
                                float friction,
                                float density) {
    auto rockObject = game->createGameObject();
    rockObject->name = "Rock";
    auto rock = rockObject->addComponent<Rock>();
    rock->initRock(position, sprite,
                   scale, EntityType::ROCK, restitution, friction, density);
    return rock;
}

shared_ptr<Rock> Level::addBrick(vec2 position, Sprite sprite, vec2 scale,
                                 float restitution,
                                 float friction,
                                 float density) {
    auto rockObject = game->createGameObject();
    rockObject->name = "Brick";
    auto rock = rockObject->addComponent<Rock>();
    rock->initRock(position, sprite,
                   scale, EntityType::BRICK, restitution, friction, density);
    return rock;
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

void Level::deleteEmerald(shared_ptr<CollectibleItem> item) {
    collectibles.erase(remove(collectibles.begin(), collectibles.end(), item), collectibles.end());
}

void Level::clearEmeralds() {
    this->collectibles.clear();
}