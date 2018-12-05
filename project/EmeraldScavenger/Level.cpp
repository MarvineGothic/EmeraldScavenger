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
int Level::emeraldsNeeded = 5;

shared_ptr<Level> Level::createDefaultLevel(EmeraldGame *game) {
    shared_ptr<Level> res = shared_ptr<Level>(new Level());
    res->game = game;
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
        default:
            this->level_test();
            break;
    }
}


void Level::level_intro() {

    game->background.initDynamicBackground("background.png");

    int width = 200;
    int height = 30;

    startPosition = vec2{1.5, 1.5};
    finishPosition = vec2{width - 2.5f, 2.4f};
	emeraldsNeeded = 1;

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    game->background.initDynamicBackground("background.png");

    // start wall
    addWall(0, 0, 2, height);

    // ceil
    addPlatform(0, height, 2, width, false);

    // floor (part 1)
    addPlatform(1, 0, 2, 130, false);

    // add some more platforms
    addPlatform(15, 4, 2, 5, false);
    addPlatform(23, 8, 2, 5, false);
    addPlatform(31, 12, 2, 5, false);
    addWall(35, 0, 2, 12);
    addPlatform(36, 5, 2, 9, false);
    addPlatform(36, 7, 2, 1, false);
    addWall(45, 0, 2, 12);
    addPlatform(45, 12, 2, 5, false);
    addPlatform(59, 12, 2, 5, false);
    addWall(64, 12, 2, 6);
    addPlatform(64, 18, 2, 17, false);
    addWall(69, 19, 2, 5);
    addWall(80, 0, 2, 18); //Put the following enemies on the platform with no movement
    /*
    addEnemy({ 75, 24 }, Enemy::EnemyType::AngryBird);
    addEnemy({ 75, 19 }, Enemy::EnemyType::Zombie);
    */

    addEnemy({100, 2}, Enemy::EnemyType::Zombie);
    addEnemy({110, 10}, Enemy::EnemyType::Dragon);
    addEnemy({120, 2}, Enemy::EnemyType::AngryBird);

    //Add some spikes here (at x = 125)


    addPlatform(135, 0, 2, 10, false); //Floor (part 2)

    auto movingPlatform = addPlatform(150, 3, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({150, 3});
    movingPlatformComponent->setMovementEnd({160, 3});
    movingPlatformComponent->setSpeed(3);

    addPlatform(170, 0, 2, width - 170, false); //Floor (part 3)

    //addCollectible({ 175, 2 }, "diamond");

    addDoor(finishPosition, false, true, 1);

    addWall(width, 0, 2, height); //End wall

	//addCollectible({ width - 5, 2 }, "diamond");
	//addCollectible({ width - 5, 2 }, "diamond");
	//addCollectible({ width - 5, 2 }, "diamond");
	//addCollectible({ width - 5, 2 }, "diamond");
	//addCollectible({ width - 5, 2 }, "diamond");

	vector<shared_ptr<CollectibleItem>> temp;
	if (collectibles.empty()) {
		collectibles.emplace_back(addCollectible(vec2(width - 5, 1.5f), "Emerald"));
	}
	else {
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
    addRock(vec2{2.5, 1.5}, EmeraldGame::gameInstance->enemiesAtlas->get("frame-1_zombie_idle.png"),
            EmeraldGame::scale * 2.5f,
            0.2f, 0.5f, 1);
    // add Brick
    addBrick(vec2{5, 2.5}, EmeraldGame::gameInstance->gameSpritesAtlas->get("spr_stoneGrassTop.png"),
             EmeraldGame::scale *2.5f,
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

//todo: design levels:
void Level::level_hub() {

    game->background.initDynamicBackground("background.png");

    int width = 50;
    int height = 50;

    startPosition = vec2{6.5, 2.5};
	emeraldsNeeded = 0;

    vec2 levelMin = {width, 0};
    vec2 levelMax = {width, height};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(5, 0, 2, height - 10);
	// end wall
	addWall(width - 5, 0, 2, height - 10);
	// floor
    addPlatform(0, 0, 2, width + 1, false);
    // ceil
    addPlatform(0, height, 2, width, false);

	addPlatform(1, height - 10, 2, 5, false);
	addPlatform(width - 5, height - 10, 2, 5, false);
	addWall(0, height - 10, 2, 10);
	addWall(width, height - 10, 2, 10);

	addDoor(vec2{ 3.5f, 2.4f }, true, false, 1);
	addDoor(vec2{ 7.5f, 2.4f }, true, true, 0);
	addDoor(vec2{ width - 2.5f, 2.4f }, true, false, 1);
	addDoor(vec2{ width - 6.5f, 2.4f }, true, true, 4);
	addDoor(vec2{ 2.5f, (width - 10) + 2.4f }, true, true, 2);
	addDoor(vec2{ width - 1.5f, (width - 10) + 2.4f }, true, true, 3);

    //Elevator 1
    auto movingPlatform = addPlatform(6, 5, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({6, 5});
    movingPlatformComponent->setMovementEnd({6, height - 5});
    movingPlatformComponent->setSpeed(10);

    //Elevator 2
    auto movingPlatform2 = addPlatform(width - 10, 5, 2, 5, true);
    auto movingPlatformComponent2 = movingPlatform2->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent2->setMovementStart({width - 10, 5});
    movingPlatformComponent2->setMovementEnd({width - 10, height - 5});
    movingPlatformComponent2->setSpeed(10);

}

void Level::level_grav() {

    game->background.initDynamicBackground("background.png");

    int width = 40;
    int height = 100;

	emeraldsNeeded = 3;

    startPosition = vec2{width - 1.5, 2.5};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(0, 0, 2, height);
    //end wall
    addWall(width, 5, 2, height - 5);
    //Middle wall
    addWall(glm::floor(width / 2), 15, 2, height - 30);
    // ceil
    addPlatform(0, height, 2, width, false);
    // floor
    addPlatform(0, 0, 2, width, false);

	//Elevator 1
	auto movingPlatform = addPlatform(glm::floor(width / 2) - 5, 2, 2, 5, true);
	auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
	movingPlatformComponent->setMovementEnd({ glm::floor(width / 2) - 5, 2 });
	movingPlatformComponent->setMovementStart({ glm::floor(width / 2) - 5, height - 10 });
	movingPlatformComponent->setSpeed(10);

	addDoor(vec2{ startPosition.x , 2.4f }, false, true, 1);

	vector<shared_ptr<CollectibleItem>> temp;
	if (collectibles.empty()) {
		collectibles.emplace_back(addCollectible(vec2(glm::floor(width / 2) + 4, 15.0f), "Emerald"));
		collectibles.emplace_back(addCollectible(vec2(width - 3, 15.0f), "Emerald"));
		collectibles.emplace_back(addCollectible(vec2(glm::floor(((glm::floor(width / 2) + 4) + (width - 3)) / 2), 15.0f), "Emerald"));
	}
	else {
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

    game->background.initDynamicBackground("background.png");

	emeraldsNeeded = 1;

    int width = 50;
    int height = 50;

    startPosition = vec2{2.5, 2.5};

    vec2 levelMin = {width + width, height - 5};
    vec2 levelMax = {width, height};

    levelWidth = static_cast<int>((width + 1) * tileSize);
    levelHeight = static_cast<int>(height * tileSize);

    // start wall
    addWall(0, 5, 2, height - 5);
    //end wall
    addWall(width, 0, 2, height);
    // ceil
    addPlatform(0, height, 2, width, false);
    // floor
    addPlatform(0, 0, 2, width, false);

	addDoor(vec2{ startPosition.x , 2.4f }, false, true, 1);
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
	emeraldsNeeded = 1;

	startPosition = vec2{ 2.5, 2.5 };
	addDoor(vec2{ startPosition.x , 2.4f }, false, true, 1);

	levelWidth = static_cast<int>((width + 1) * tileSize);
	levelHeight = static_cast<int>(height * tileSize);

	// start wall
	addWall(0, 0, 2, height);
	// end wall
	addWall(width, 0, 2, height);
	// floor
	addPlatform(0, 0, 2, 10, false);
	// ceil
	addPlatform(0, height, 2, width, false);

	//Defines how far apart platforms are allowed to be
	glm::vec2 min(2, -5);
	glm::vec2 max(6, 5);

	int max_length;
	int min_length;

	//Defines how long platforms are allowed to be
	if (game->livesCounter == 5) {
		max_length = 4;
		min_length = 1;
	}
	else if (game->livesCounter < 5 && game->livesCounter > 2) {
		max_length = 5;
		min_length = 2;
	}
	else {
		max_length = 6;
		min_length = 3;
	}

	float floor = 1;
	float ceil = 15;
	float wall_l = 0;
	float wall_r = 200;

	//Initial platform that starts chain
	addPlatform(15, 5, 2, 5, false);
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
		int rand_x = static_cast<int>((rand() % (int)((max.x - min.x) + 1)) + min.x + prev_platform.x);
		int rand_y = clamp(static_cast<int>((rand() % (int)((max.y - min.y) + 1)) + min.y + prev_platform.y), floor, ceil);
		addPlatform(rand_x, rand_y, 2, length, false);
		prev_platform = glm::vec2(rand_x + length, rand_y);
	}
	addPlatform(width - 20, 0, 2, 20, false);

	vector<shared_ptr<CollectibleItem>> temp;
	if (collectibles.empty()) {
		collectibles.emplace_back(addCollectible(vec2(width - 5, 1.5f), "Emerald"));
	}
	else {
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
	auto movingPlatform = addPlatform(width - 15, 2, 2, 5, true);
	auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
	movingPlatformComponent->setMovementStart({ width - 22, 5 });
	movingPlatformComponent->setMovementEnd({ width - 22, ceil });
	movingPlatformComponent->setSpeed(4);

}

void Level::level_test() {
	game->background.initDynamicBackground("background.png");

	int width = 30;
	int height = 30;

	startPosition = vec2{ 28, 5 };
	finishPosition = vec2{ width - 2.5f, 2.4f };

	levelWidth = static_cast<int>((width + 1) * tileSize);
	levelHeight = static_cast<int>(height * tileSize);

	game->background.initDynamicBackground("background.png");

	addWall(0, 0, 2, height); //start wall
	addPlatform(0, 0, 2, width, false); //Floor

	addEnemy({ 28, 10 }, Enemy::EnemyType::Boulder);


	addWall(width, 0, 2, height); //end wall
}

void Level::level_bonus_1() {

    int width = 205;
    int height = 51;

    startPosition = vec2{2.5, 2.4};
    finishPosition = vec2{width - 2.5f, 2.4f};
	emeraldsNeeded = 5;
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
    addDoor(startPosition, true, false, 0);
    addDoor(finishPosition, false, true, 0);
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

void Level::level_bonus_2() {
    int width = 205;
    int height = 51;

    startPosition = vec2{200.0, height - 3.5};
    finishPosition = vec2{width - 2.5f, 2.4f};
	emeraldsNeeded = 5;
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
    addDoor(startPosition, true, false, 0);
    addDoor(finishPosition, false, true, 0);

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

void Level::generateLevel() {
    auto movingPlatform = addPlatform(10, 3, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatform->getPhysicsComponent()->setLinearVelocity({0, 100});
    b2Body *body = movingPlatform->getPhysicsComponent()->getBody();
    movingPlatformComponent->setBody(body);
    movingPlatformComponent->setMovementStart({10, 3});
    movingPlatformComponent->setMovementEnd({10, 5});

    //Procedurally generated level
    level_proc();
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

shared_ptr<Door> Level::addDoor(vec2 position, bool isOpen, bool isExit, int level) {
    auto doorObject = game->createGameObject();
    doorObject->name = "Door";
    auto door = doorObject->addComponent<Door>();
    door->initDoor(position, isOpen, isExit, level);
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

int Level::getEmeraldsNeeded() {
	return emeraldsNeeded;
}

void Level::deleteEmerald(shared_ptr<CollectibleItem> item) {
    collectibles.erase(remove(collectibles.begin(), collectibles.end(), item), collectibles.end());
}

void Level::clearEmeralds() {
    this->collectibles.clear();
}