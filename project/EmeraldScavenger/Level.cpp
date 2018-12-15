#include <utility>

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
#include <set>

vec2 Level::startPosition = {};
vec2 Level::finishPosition = {};
int Level::emeraldsNeeded = 5;
bool Level::doorIsOpen = false;

shared_ptr<Level> Level::createDefaultLevel(EmeraldGame *game) {
    shared_ptr<Level> res = shared_ptr<Level>(new Level());
    res->game = game;
    // platform sprites:
    res->ground = EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_2.png");
    res->brick = EmeraldGame::gameInstance->obstaclesAtlas->get("brick.png");
    res->wood = EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_1.png");
    res->blue = EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_3.png");
    res->orange = EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_4.png");
    res->green = EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_5.png");
    res->brown = EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_6.png");
    return res;
}

void Level::makeLevel(int level) {
    switch (level) {
        case 0:
            this->level_intro();
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
    doorIsOpen = false;
    EmeraldGame::gameInstance->audioManager->playMusic("bgMusic.mp3", 4);
    int width = 200;
    int height = 30;
    game->background.initDynamicBackground("fence_background.png", height, width);

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{1.5, 1.5};
        //startPosition = finishPosition;
    }
    finishPosition = vec2{width - 2.5f, 2.4f};
    emeraldsNeeded = 1;

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    //addFlag(vec2{ 5,5 }, false);

    // start wall
    addWall(0, 0, brick, height);

    // ceil
    addPlatform(0, height, wood, width, false);

    // floor (part 1)
    addPlatform(1, 0, wood, 130, false);

    // add some more platforms
    addPlatform(15, 4, wood, 5, false);
    addPlatform(23, 8, wood, 5, false);
    addPlatform(31, 12, wood, 5, false);
    addWall(35, 0, brick, 12);
    addPlatform(36, 5, wood, 9, false);
    addPlatform(36, 7, wood, 1, false);
    addWall(45, 0, brick, 12);
    addPlatform(45, 12, wood, 5, false);
    addWall(49, 0, brick, 12);
    addPlatform(50, 5, wood, 9, false);
    addPlatform(50, 7, wood, 1, false);
    addWall(59, 0, brick, 12);
    addPlatform(59, 12, wood, 5, false);
    addWall(64, 12, brick, 5);
    addPlatform(64, 17, wood, 17, false);
    addWall(69, 18, brick, 5);
    addWall(80, 0, brick, 18); //Put the following enemies on the platform with no movement
    /*
    addEnemy({ 75, 24 }, Enemy::EnemyType::AngryBird);
    addEnemy({ 75, 19 }, Enemy::EnemyType::Zombie);
    */

    addEnemy({100, 2}, Enemy::EnemyType::Zombie);
    addEnemy({110, 10}, Enemy::EnemyType::Dragon);
    addEnemy({120, 2}, Enemy::EnemyType::AngryBird);

    //Add some spikes here (at x = 125)


    addPlatform(135, 0, wood, 10, false); //Floor (part 2)

    auto movingPlatform = addPlatform(150, 3, wood, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({150, 3});
    movingPlatformComponent->setMovementEnd({160, 3});
    movingPlatformComponent->setSpeed(2);

    addPlatform(170, 0, wood, width - 170, false); //Floor (part 3)

    //addCollectible({ 175, 2 }, "diamond");


    addDoor(finishPosition, false, true, 1, vec2{6.5, 2.5}); //Hub-world bottom-left door

    addWall(width, 0, brick, height); //End wall

    //addCollectible({ width - 5, 2 }, "diamond");
    //addCollectible({ width - 5, 2 }, "diamond");
    //addCollectible({ width - 5, 2 }, "diamond");
    //addCollectible({ width - 5, 2 }, "diamond");
    //addCollectible({ width - 5, 2 }, "diamond");

    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
        collectibles.emplace_back(addCollectible(vec2(width - 5, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(width - 10, 1.5f), "Pie"));
    } else if (!collectibles.empty()) {
        for (auto &cItem : collectibles) {
            temp.emplace_back(addCollectible(cItem->getPosition(), cItem->getName()));
        }
        collectibles.clear();
        for (auto &cItem : temp) {
            collectibles.emplace_back(cItem);
        }
        temp.clear();
    }

    // add rock
    addRock(vec2{5.5, 5.5}, EmeraldGame::gameInstance->obstaclesAtlas->get("boulder.png"),
            EmeraldGame::scale * 2.5f,
            0.2f, 0.5f, 1);
    // add Brick
    addBrick(vec2{5, 2.5}, EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_4.png"),
             EmeraldGame::scale,
             0.2f, 1, 1);

    /*
    auto movingPlatform = addPlatform(10, 3, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({ 10, 3 });
    movingPlatformComponent->setMovementEnd({ 10, 10 });
    // add some enemies to the level
    addEnemy({ 50, 2 }, Enemy::EnemyType::Zombie);
    addEnemy({ 25, 2 }, Enemy::EnemyType::Zombie);
    addEnemy({ 75, 2 }, Enemy::EnemyType::Zombie);
    addEnemy({ 100, 2 }, Enemy::EnemyType::Zombie);
    addEnemy({ 75, 10 }, Enemy::EnemyType::AngryBird);
    addEnemy({ 100, 5 }, Enemy::EnemyType::Dragon);
    addEnemy({ 30, 5 }, Enemy::EnemyType::AngryBird);
    addEnemy({ 50, 5 }, Enemy::EnemyType::Dragon);
*/

}

void Level::level_hub() {
    doorIsOpen = false;
    EmeraldGame::gameInstance->audioManager->playMusic("gravitySoundtrack.mp3", 4);

    int width = 50;
    int height = 50;
    game->background.initDynamicBackground("background.png", width, height);
    
    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{6.5, 2.5};
    }
    emeraldsNeeded = 0;

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(5, 0, brick, height - 10);
    // end wall
    addWall(width - 5, 0, brick, height - 10);
    // floor
    addPlatform(0, 0, ground, width + 1, false);
    // ceil
    addPlatform(0, height, ground, width, false);


    addPlatform(15, height - 10, ground, width - 29, false);

    addPlatform(1, height - 10, ground, 5, false);
    addPlatform(width - 5, height - 10, ground, 5, false);
    addWall(0, height - 10, brick, 10);
    addWall(width, height - 10, brick, 10);

    addDoor(vec2{3.5f, 2.4f}, true, false, 1, vec2{NULL, NULL});
    addDoor(vec2{7.5f, 2.4f}, true, true, 0, vec2{200 - 2.5f, 2.4f}); //intro level's finishPosition
    addFlag(vec2{7.6f, 4.75f}, EmeraldGame::introCleared);

    addDoor(vec2{width - 2.5f, 2.4f}, true, false, 1, vec2{NULL, NULL});
    addDoor(vec2{width - 6.5f, 2.4f}, true, true, 4, vec2{2.5, 2.5}); //procedural level's startPosition
    addFlag(vec2{width - 6.4f, 4.75f}, EmeraldGame::procCleared);

    addDoor(vec2{2.5f, (height - 10) + 2.4f}, true, true, 2, vec2{40 - 1.5, 2.5}); //gravity level's startPosition;
    addFlag(vec2{2.6f, (height - 10) + 4.75f}, EmeraldGame::gravCleared);

    addDoor(vec2{width - 1.5f, (height - 10) + 2.4f}, true, true, 3, vec2{2.5, 2.5}); //physics level's startPosition
    addFlag(vec2{width - 1.4f, (height - 10) + 4.75f}, EmeraldGame::physCleared);

    //Elevator 1
    auto movingPlatform = addPlatform(6, 5, ground, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementEnd({6, 5});
    movingPlatformComponent->setMovementStart({6, height - 10});
    movingPlatformComponent->setSpeed(5);

    //Elevator 2
    movingPlatform = addPlatform(width - 10, 5, ground, 5, true);

    movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementEnd({width - 10, 5});
    movingPlatformComponent->setMovementStart({width - 10, height - 10});
    movingPlatformComponent->setSpeed(5);

    /*
    addFlag(vec2{ width / 2 - 3, 5 }, EmeraldGame::introCleared);
    addFlag(vec2{ width / 2 - 1, 5 }, EmeraldGame::gravCleared);
    addFlag(vec2{ width / 2 + 1, 5 }, EmeraldGame::physCleared);
    addFlag(vec2{ width / 2 + 3, 5 }, EmeraldGame::procCleared);
    */

    addFlag(vec2{width / 2 - 3, 4.8}, EmeraldGame::introCleared);
    addFlag(vec2{width / 2 - 1, 4.8}, EmeraldGame::gravCleared);
    addFlag(vec2{width / 2 + 1, 4.8}, EmeraldGame::physCleared);
    addFlag(vec2{width / 2 + 3, 4.8}, EmeraldGame::procCleared);

    bool finalDoorIsOpen = EmeraldGame::introCleared && EmeraldGame::gravCleared && EmeraldGame::physCleared &&
                           EmeraldGame::procCleared;
    addDoor(vec2{width / 2, 2.4f}, finalDoorIsOpen, finalDoorIsOpen, 6, vec2{2.5, 47.5});

    if (EmeraldGame::bonusCleared) {
        vector<shared_ptr<CollectibleItem>> temp;
        if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
            collectibles.emplace_back(addCollectible(vec2(width / 2, height - 5), "Emerald"));
        } else if (!collectibles.empty()) {
            for (auto &cItem : collectibles) {
                temp.emplace_back(addCollectible(cItem->getPosition(), cItem->getName()));
            }
            collectibles.clear();
            for (auto &cItem : temp) {
                collectibles.emplace_back(cItem);
            }
            temp.clear();
        }
    }
}

void Level::level_grav() {
    doorIsOpen = false;
    EmeraldGame::gameInstance->audioManager->playMusic("gravitySoundtrack.mp3", 4);

    int width = 40;
    int height = 100;
    game->background.initDynamicBackground("gravity_background.png", height, width);

    emeraldsNeeded = 4;

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{width - 1.5, 2.5};
    }

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(0, 0, brick, height);
    //end wall
    addWall(width, 0, brick, height);
    //Middle wall
    addWall(static_cast<int>(floor(width / 2)), 15, brick, height - 20);
    //Right bottom wall
    addPlatform(static_cast<int>(floor(width / 2)), 15, ground, 9, false);
    addPlatform(static_cast<int>(floor(width) - 9), 15, ground, 9, false);
    // ceil
    addPlatform(0, height - 1, ground, width, false);
    // floor
    addPlatform(0, 0, ground, width, false);

    // enemies
    addEnemy(vec2(floor(width / 2) - 3, 56.0f), Enemy::EnemyType::Dragon);
    addEnemy(vec2(floor(width / 2) - 1, 90.0f), Enemy::EnemyType::Dragon);
    addEnemy(vec2(floor(width / 2) + 4, 90.0f), Enemy::EnemyType::Dragon);
    addEnemy(vec2(floor(width / 2) - 3, 25.0f), Enemy::EnemyType::Dragon);
    addEnemy(vec2(floor(width / 2) - 2, 50.0f), Enemy::EnemyType::Dragon);
    addEnemy(vec2(floor(width / 2) - 4, 75.0f), Enemy::EnemyType::Dragon);

    //Elevator 1
    auto movingPlatform = addPlatform(static_cast<int>(floor(width / 2) - 5), 2, ground, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementEnd({floor(width / 2) - 5, 2});
    movingPlatformComponent->setMovementStart({floor(width / 2) - 5, 10});
    movingPlatformComponent->setSpeed(5);

    //Static Platforms
    addPlatform(floor(width / 2) - 17, 15, ground, 5, false);
    addPlatform(floor(width / 2) - 5, 22, ground, 5, false);
    addPlatform(floor(width / 2) - 18, 30, ground, 3, false);
    addPlatform(floor(width / 2) - 3, 39, ground, 2, false);
    addPlatform(floor(width / 2) - 17, 49, ground, 3, false);
    addPlatform(floor(width / 2) - 4, 60, ground, 3, false);
    addPlatform(floor(width / 2) - 16, 72, ground, 3, false);
    addPlatform(floor(width / 2) - 5, 85, ground, 4, false);
    addPlatform(floor(width / 2) - 5, 95, ground, 4, false);
    //Reverse gravity platforms
    addPlatform(floor(width / 2) + 5, 94, ground, 3, false);
    //Elevator 2
    auto movingPlatform2 = addPlatform(static_cast<int>(floor(width / 2) - 5), 2, ground, 5, true);
    auto movingPlatformComponent2 = movingPlatform2->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent2->setMovementEnd({floor(width / 2) + 8, 90});
    movingPlatformComponent2->setMovementStart({floor(width / 2) + 10, 78});
    movingPlatformComponent2->setSpeed(5);
    addPlatform(floor(width / 2) + 15, 78, ground, 4, false);
    //Elevator 3
    auto movingPlatform3 = addPlatform(static_cast<int>(floor(width / 2) - 5), 2, ground, 5, true);
    auto movingPlatformComponent3 = movingPlatform3->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent3->setMovementEnd({floor(width / 2) + 9, 76});
    movingPlatformComponent3->setMovementStart({floor(width / 2) + 2, 58});
    movingPlatformComponent3->setSpeed(5);
    addPlatform(floor(width / 2), 58, ground, 2, false);
    //Elevator 4
    auto movingPlatform4 = addPlatform(static_cast<int>(floor(width / 2) - 5), 2, ground, 5, true);
    auto movingPlatformComponent4 = movingPlatform4->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent4->setMovementEnd({floor(width / 2) + 4, 57});
    movingPlatformComponent4->setMovementStart({floor(width / 2) + 15, 30});
    movingPlatformComponent4->setSpeed(5);
    addPlatform(floor(width / 2) + 10, 26, ground, 5, false);
    addPlatform(floor(width / 2) + 5, 22, ground, 5, false);
    addPlatform(floor(width / 2) + 10, 18, ground, 5, false);

    addDoor(vec2{startPosition.x, 2.4f}, false, true, 1, vec2{2.5f, (50 - 10) + 2.4f}); //Top-left hub world door

    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
        collectibles.emplace_back(addCollectible(vec2(floor(width / 2) + 11, 23), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(floor(width / 2) - 3, 97), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(floor(width / 2) - 5, 97), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(width - 3, 14.0f), "Emerald"));
        collectibles.emplace_back(
                addCollectible(vec2(floor(((floor(width / 2) + 4) + (width - 3)) / 2), 14.0f), "Emerald"));
    } else if(!collectibles.empty()){
        for (auto &cItem : collectibles) {
            temp.emplace_back(addCollectible(cItem->getPosition(), cItem->getName()));
        }
        collectibles.clear();
        for (auto &cItem : temp) {
            collectibles.emplace_back(cItem);
        }
        temp.clear();
    }

}

void Level::level_phys() {
    doorIsOpen = false;
    emeraldsNeeded = 5;

    int width = 70;
    int height = 70;
    game->background.initDynamicBackground("gravity_background.png", height, width);

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{ NULL , NULL }) {
		startPosition = vec2{3.5, 2.5}; //Real starting position
		//startPosition = vec2{ 15, 61.5 }; //This one's just here for quicker testing
	}


    vec2 levelMin = {width + width, height - 5};
    vec2 levelMax = {width, height};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall

    addWall(0, 0, brick, height);

    //end wall
    addWall(width, 0, brick, height);
    // ceil
    addPlatform(0, height, ground, width, false);
    // floor
    addPlatform(0, 0, ground, width, false);

    addWall(1, 1, brick, 1);
    addDoor(vec2{startPosition.x, 2.4f}, false, true, 1, vec2{50 - 1.5f, (50 - 10) + 2.4f}); //Top-right hub-world door


    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
        collectibles.emplace_back(addCollectible(vec2(width - 5, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(3.5f, 11.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(36.5f, 40.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(40.0f, 58.0f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(38.0f, 58.0f), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(width - 4.5f, 68.5f), "Emerald"));
    } else if(!collectibles.empty()){
        for (auto &cItem : collectibles) {
            temp.emplace_back(addCollectible(cItem->getPosition(), cItem->getName()));
        }
        collectibles.clear();
        for (auto &cItem : temp) {
            collectibles.emplace_back(cItem);
        }
        temp.clear();
    }

    addPlatform(1, 10, ground, 5, false);
    addPlatform(1, 30, ground, 40, false);
    addWall(41, 30, brick, 9);
    addWall(41, 45, brick, 15);
    addPlatform(33, 38, ground, 8, false);
    addPlatform(33, 45, ground, 8, false);
    addWall(33, 39, brick, 6);

    addWall(37, 46, brick, 2);
    addBrick(vec2{39.5, 46}, EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_6.png"),
             EmeraldGame::scale * 2.0f,
             0.7f, 1, 1);

    addPlatform(20, 5, ground, 5, false);
    addPlatform(25, 10, ground, width - 25, false);

    addEnemy({30, 2}, Enemy::EnemyType::Zombie);
    addEnemy({35, 2}, Enemy::EnemyType::Zombie);
    addEnemy({40, 2}, Enemy::EnemyType::Zombie);
    addEnemy({45, 2}, Enemy::EnemyType::Zombie);
    addEnemy({50, 2}, Enemy::EnemyType::Zombie);
    addEnemy({55, 2}, Enemy::EnemyType::Zombie);
    addEnemy({60, 2}, Enemy::EnemyType::Zombie);


    //Elevator 1
    auto movingPlatform = addPlatform(static_cast<int>(width - 12.5), 12, ground, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({width - 12.5, 12});
    movingPlatformComponent->setMovementEnd({width - 12.5, 24});
    movingPlatformComponent->setSpeed(2);
    movingPlatform->getPhysicsComponent()->getFixture()->SetRestitution(0.2);

    //Elevator 2

    movingPlatform = addPlatform(static_cast<int>(width - 22.5), 42, ground, 5, true);
    movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({width - 22.5, 36});
    movingPlatformComponent->setMovementEnd({width - 22.5, 24});
    movingPlatformComponent->setSpeed(2);

    //Elevator 3
    movingPlatform = addPlatform(static_cast<int>(width - 12.5), 42, ground, 5, true);
    movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({width - 12.5, 36});
    movingPlatformComponent->setMovementEnd({width - 12.5, 48});
    movingPlatformComponent->setSpeed(2);
    movingPlatform->getPhysicsComponent()->getFixture()->SetRestitution(0.2);

    //Elevator 4
    movingPlatform = addPlatform(static_cast<int>(width - 22.5), 62, ground, 5, true);
    movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({width - 22.5, 60});
    movingPlatformComponent->setMovementEnd({width - 22.5, 48});
    movingPlatformComponent->setSpeed(2);


    addPlatform(15, 59, ground, 26, false);
    addPlatform(11, 67, ground, width - 11, false);
    addWall(15, 57, brick, 6);
    addPlatform(11, 57, ground, 4, false);

    addBrick(vec2{19, 62}, EmeraldGame::gameInstance->obstaclesAtlas->get("tiles_3.png"),
             EmeraldGame::scale * 1.25f,
             0.7f, 1, 1);

    //Elevator 5
    movingPlatform = addPlatform(1, 37, ground, 5, true);
    movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({1, 37});
    movingPlatformComponent->setMovementEnd({1, 61});
    movingPlatformComponent->setSpeed(5);

    addRock(vec2{12, 40}, EmeraldGame::gameInstance->obstaclesAtlas->get("boulder.png"),
            EmeraldGame::scale * 10.0f,
            0.2f, 0.5f, 1);
}

//clamps a value with a lower and upper bound
float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

enum class Section {
	Normal,
	Stairs,
	Drop
};

//Procedurally generates a level.
void Level::level_proc() {
    doorIsOpen = false;

    int width = 200;
    int height = 28;
    game->background.initDynamicBackground("fence_background.png", height, width);
    emeraldsNeeded = 1;

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{2.5, 2.5};
    }
    addDoor(vec2{startPosition.x, 2.4f}, false, true, 1, vec2{50 - 6.5f, 2.4f}); //Bottom-right hub-world door

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

	//addEnemy({ 5, 5 }, Enemy::EnemyType::SpikeMonster);

    // start wall
    addWall(0, 0, brick, height + 1);
    // end wall
    addWall(width, 0, brick, height + 1);
    // floor
    addPlatform(0, 0, wood, 10, false);
    // ceil
    addPlatform(0, height+1, wood, width, false);

    //Defines how far apart platforms are allowed to be
    vec2 init_min(2, -5);
    vec2 init_max(6, 5);

    int init_max_length;
    int init_min_length;

	int enemyPlatformMinLength;
	int enemyFrequencyPercent;
	int flyingEnemyFrequencyPercent;

    //Defines difficulty parameters based on number of lives left
    if (game->livesCounter > 4) {
        init_max_length = 4;
		init_min_length = 1;
		enemyPlatformMinLength = 3;
		enemyFrequencyPercent = 50;
		flyingEnemyFrequencyPercent = 75;
    } 
	else if (game->livesCounter == 4) {
		init_max_length = 5;
		init_min_length = 2;
		enemyPlatformMinLength = 4;
		enemyFrequencyPercent = 30;
		flyingEnemyFrequencyPercent = 50;
	}
	else if (game->livesCounter == 3) {
		init_max_length = 5;
		init_min_length = 2;
		enemyPlatformMinLength = 4;
		enemyFrequencyPercent = 20;
		flyingEnemyFrequencyPercent = 25;
	}
	else if (game->livesCounter == 2) {
		init_max_length = 6;
		init_min_length = 3;
		enemyPlatformMinLength = 5;
		enemyFrequencyPercent = 10;
		flyingEnemyFrequencyPercent = 10;
	}
	else {
		init_max_length = 6;
		init_min_length = 3;
		enemyPlatformMinLength = 5;
		enemyFrequencyPercent = 0; //No enemies on platforms
		flyingEnemyFrequencyPercent = 0; //No flying enemies
	}
    float floor = 1;
    float ceil = 15;
    float wall_l = 0;
    float wall_r = 200;

    //Initial platform that starts chain
    addPlatform(15, 5, wood, 5, false);
    vec2 prev_platform = vec2(15 + 5, 5);

	int max_length = init_max_length;
	int min_length = init_min_length;
	vec2 min = init_min;
	vec2 max = init_max;
	Section section = Section::Normal;

	//Simpler version
	/*
	while (prev_platform.x < (width - 30)) {
		int length = (rand() % max_length) + min_length;
		int rand_x = static_cast<int>((rand() % (int) ((max.x - min.x) + 1)) + min.x + prev_platform.x);
		int rand_y = clamp(static_cast<int>((rand() % (int) ((max.y - min.y) + 1)) + min.y + prev_platform.y), floor, ceil);
		addPlatform(rand_x, rand_y, ground, length, false);
		prev_platform = vec2(rand_x + length, rand_y);
	}
	addPlatform(width - 20, 0, ground, 20, false);
	*/

	//Simpler version V2
	std::set<int> heights = {};
	while (prev_platform.x < (width - 30)) {
		int length = (rand() % max_length) + min_length;
		int min_y = glm::max(min.y, floor - prev_platform.y); //Finds the smaller of max.y or the distance to the ceiling
		int max_y = glm::min(max.y, ceil - prev_platform.y); //Finds the larger of min.y or the distance to the floor
		int rand_x = static_cast<int>((rand() % (int)((max.x - min.x) + 1)) + min.x + prev_platform.x);
		int rand_y = clamp(static_cast<int>((rand() % (int)((max_y - min_y) + 1)) + min_y + prev_platform.y), floor, ceil);
		addPlatform(rand_x, rand_y, ground, length, false);

		//Add flying enemy
		//If this platform isn't at the same height as the previous platform
		if (rand_y != prev_platform.y) {
			//If another platform exists in the same height
			const bool duplicate = heights.find(rand_y) != heights.end();
			if (duplicate) {
				if ((rand() % 100) < flyingEnemyFrequencyPercent) {
					addEnemy(vec2{ rand_x - 2, rand_y + 0.53 }, Enemy::EnemyType::Dragon);
				}
			}
			else {
				heights.insert(rand_y);
			}
		}

		//Add enemy on platform
		//If platform is long enough to have an enemy
		if (length >= enemyPlatformMinLength) {
			//enemyFrequencyPercent chance of placing an enemy
			if ((rand() % 100) < enemyFrequencyPercent) {
				auto enemy = addEnemy(vec2{ (rand_x + length/2), prev_platform.y + 2}, Enemy::EnemyType::Zombie);
			}
		}
		prev_platform = vec2(rand_x + length, rand_y);
	}
	

	//More advanced version (Unfinished - I stopped working on it since you need to be able to get back across the level, which this doesn't allow)
	/*
	int i = 0;
	int special = 0;
	while (prev_platform.x < (width - 30)) {
		switch (section) {
		case Section::Normal:
			int length = (rand() % max_length) + min_length;
			int rand_x = static_cast<int>((rand() % (int)((max.x - min.x) + 1)) + min.x + prev_platform.x);
			int rand_y = clamp(static_cast<int>((rand() % (int)((max.y - min.y) + 1)) + min.y + prev_platform.y), floor, ceil);
			addPlatform(rand_x, rand_y, ground, length, false);
			prev_platform = vec2(rand_x + length, rand_y);
			break;
		case Section::Stairs:
			min = vec2{ init_min.x, 2 };
			max = vec2{ init_max.x, 5 };
			special = 3;
			break;
		case Section::Drop:
			min = vec2{ init_min.x, -8 };
			max = vec2{ init_max.x, -ceil };
			special = 1;
			break;
		}

		special--;
		//Reset parameters if special event is over
		if (special == 0) {
			max_length = init_max_length;
			min_length = init_min_length;
			min = init_min;
			max = init_max;
			section = Section::Normal;
		}

		//Stairs section
		if (special == 0 && prev_platform.y < floor + ((ceil - floor) / 2)) {
			section = Section::Stairs;
			min = vec2{ init_min.x, 2 };
			max = vec2{ init_max.x, 5 };
			special = 3;
		}

		//Drop section
		else if (special == 0 && prev_platform.y > floor + ((ceil - floor) / 2)) {
			section = Section::Drop;
			min = vec2{ init_min.x, -8 };
			max = vec2{ init_max.x, -ceil };
			special = 1;
		}
		else {
			section = Section::Normal;
			int length = (rand() % max_length) + min_length;
			int rand_x = static_cast<int>((rand() % (int)((max.x - min.x) + 1)) + min.x + prev_platform.x);
			int rand_y = clamp(static_cast<int>((rand() % (int)((max.y - min.y) + 1)) + min.y + prev_platform.y), floor, ceil);
			addPlatform(rand_x, rand_y, ground, length, false);
			prev_platform = vec2(rand_x + length, rand_y);
			break;
		}
	}
	addPlatform(width - 20, 0, ground, 20, false);
	*/

	addPlatform(width - 20, 0, ground, 20, false);

    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
        collectibles.emplace_back(addCollectible(vec2(width - 5, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(width - 22, ceil + 5), "Pie"));
    } else if(!collectibles.empty()){
        for (auto &cItem : collectibles) {
            temp.emplace_back(addCollectible(cItem->getPosition(), cItem->getName()));
        }
        collectibles.clear();
        for (auto &cItem : temp) {
            collectibles.emplace_back(cItem);
        }
        temp.clear();
    }

    //Elevator
    auto movingPlatform = addPlatform(width - 15, 2, wood, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({width - 22, 5});
    movingPlatformComponent->setMovementEnd({width - 22, ceil});
    movingPlatformComponent->setSpeed(4);

}

void Level::level_test() {
    doorIsOpen = false;

    int width = 30;
    int height = 30;
    game->background.initDynamicBackground("fence_background.png", height, width);

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{28, 5};
    }
    finishPosition = vec2{width - 2.5f, 2.4f};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    game->background.initDynamicBackground("fence_background.png", height, width);

    addWall(0, 0, brick, height); //start wall
    addPlatform(0, 0, wood, width, false); //Floor

    addEnemy({28, 10}, Enemy::EnemyType::Boulder);


    addWall(width, 0, brick, height); //end wall
}

void Level::level_bonus_0() {
    doorIsOpen = false;
    EmeraldGame::gameInstance->audioManager->playMusic("bgMusic.mp3", 4);

    int width = 150;
    int height = 50;
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    emeraldsNeeded = 5;

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{2.5, 47.5};
    }
    addDoor(startPosition, true, false, 1, vec2{50 / 2, 2.4f}); //hub-world's middle
    finishPosition = vec2{12.5, 47.5};
    addDoor(finishPosition, false, true, 7, vec2{2.5, 2.4}); //Level_bonus_1's startPosition

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
    addPlatform(11, 20, ground, 5, false);    //15
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
    addPlatform(130, 0, ground, width - 130, false);


    vector<shared_ptr<CollectibleItem>> temp;
    if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
        collectibles.emplace_back(addCollectible(vec2(13, 42.5), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(13, 22.5), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(72.5, 7.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(135, 2.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(145, 2.5f), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(70, 32.5f), "Emerald"));
    } else if(!collectibles.empty()){
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
    addBrick(vec2{13, 32.5}, EmeraldGame::gameInstance->gameSpritesAtlas->get("spr_wood.png"),
             EmeraldGame::scale * 5.0f, 0.8f, 1, 1);


    addEnemy({100, 2}, Enemy::EnemyType::Zombie);
    addEnemy({110, 10}, Enemy::EnemyType::Dragon);
    addEnemy({120, 2}, Enemy::EnemyType::AngryBird);

    //Add some spikes here (at x = 125)

    addWall(width, 0, brick, height); //End wall

    // add rock
    /*addRock(vec2{2.5, 1.5}, EmeraldGame::gameInstance->gameSpritesAtlas->get("frame-1_zombie_idle.png"),
            EmeraldGame::scale * 2.5f,
            0.2f, 0.5f, 1);*/
}

void Level::level_bonus_1() {
    doorIsOpen = false;
    int width = 205;
    int height = 51;

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{2.5, 2.4};
    }
    finishPosition = vec2{width - 2.5f, 2.4f};
    emeraldsNeeded = 5;
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png", height, width);

    // start wall
    addWall(0, 0, brick, height);
    // floor
    addPlatform(1, 0, wood, width - 200, false);
    // gap = 5
    addPlatform(10, 0, wood, width - 115, false);
    // gap = 5
    addPlatform(105, 0, wood, width - 105, false);
    // ceil
    addPlatform(1, height - 1, wood, width - 1, false);


    auto movingPlatform = addPlatform(10, 3, wood, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({10, 3});
    movingPlatformComponent->setMovementEnd({10, 10});

    // doors:
    addDoor(startPosition, true, false, 6, vec2{12.5, 47.5}); //level_bonus_0's finishPosition
    addDoor(finishPosition, false, true, 8, vec2{200, 51 - 3.5}); //level_bonus_2's startPosition
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
    if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
        collectibles.emplace_back(addCollectible(vec2(15, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(25, 1.5f), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(95, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(105, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(115, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(200, 1.5f), "Emerald"));
    } else if(!collectibles.empty()){
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
    doorIsOpen = false;
    int width = 205;
    int height = 51;

    startPosition = EmeraldGame::currentStartPosition;
    if (startPosition == vec2{NULL, NULL}) {
        startPosition = vec2{200.0, height - 3.5};
    }
    finishPosition = vec2{width - 2.5f, 2.4f};
    emeraldsNeeded = 5;
    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);
    game->background.initDynamicBackground("background.png", width, height);

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
    addDoor(startPosition, true, false, 7, vec2{205 - 2.5, 2.4}); //level_bonus_1's finishPosition
    addDoor(finishPosition, false, true, 1, vec2{50 / 2, 2.4}); //Hub-world's middle position

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
    if (collectibles.empty() && EmeraldGame::gameInstance->getEmeraldCounter() == 0) {
        collectibles.emplace_back(addCollectible(vec2(25, 1.5f), "Pie"));
        collectibles.emplace_back(addCollectible(vec2(75, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(85, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(95, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(105, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(115, 1.5f), "Emerald"));
        collectibles.emplace_back(addCollectible(vec2(200, 1.5f), "Emerald"));
    } else if(!collectibles.empty()){
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
    auto res = gameObject->addComponent<Enemy>();
    if (enemyType == Enemy::EnemyType::Boulder) {
        gameObject->name = "Boulder";
        res->initEnemy(EmeraldGame::gameInstance->obstaclesAtlas, pos, enemyType);
    } else {
        gameObject->name = "Enemy";
        res->initEnemy(EmeraldGame::gameInstance->obstaclesAtlas, pos, enemyType);
    }
    return res;
}

shared_ptr<Door> Level::addDoor(vec2 position, bool isOpen, bool isExit, int level, vec2 nextLevelStartPosition) {
    auto doorObject = game->createGameObject();
    doorObject->name = "Door";
    auto door = doorObject->addComponent<Door>();
    door->initDoor(position, isOpen, isExit, level, nextLevelStartPosition);
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
    collectible->initCollectible(position, move(name));
    collectible->gameObjectCopy = collectibleObject;
    return collectible;
}

shared_ptr<GameObject> Level::addFlag(vec2 position, bool cleared) {
    auto flagObject = game->createGameObject();

    flagObject->setPosition(position * tileSize);

    auto sprite = flagObject->addComponent<SpriteComponent>();
    auto sprites = flagObject->addComponent<SpriteAnimationComponent>();

    sprite->setSprite(EmeraldGame::gameInstance->platformerArtAtlas->get("312.png"));

    string img1, img2;
    if (cleared) {
        img1 = "313.png";
        img2 = "343.png";
    } else {
        img1 = "312.png";
        img2 = "342.png";
    }

    vector<Sprite> spriteAnim({EmeraldGame::gameInstance->platformerArtAtlas->get(img1),
                               EmeraldGame::gameInstance->platformerArtAtlas->get(img2)});
    for (auto &s : spriteAnim) {
        s.setScale({2, 2});
    }
    sprites->setSprites(spriteAnim);
    sprites->setAnimationTime(0.1);

    return flagObject;
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

int Level::getEmeraldsNeeded() {
    return emeraldsNeeded;
}

void Level::deleteEmerald(shared_ptr<CollectibleItem> item) {
    collectibles.erase(remove(collectibles.begin(), collectibles.end(), item), collectibles.end());
}

void Level::clearEmeralds() {
    this->collectibles.clear();
}
