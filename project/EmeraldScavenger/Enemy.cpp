//
// Created by Admin on 05.11.2018.
//

#include <SDL_events.h>
#include <iostream>
#include "Enemy.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "EmeraldGame.hpp"
#include "SpriteComponent.hpp"

const vec2 Enemy::dragonScale(0.2f, 0.2f);
const vec2 Enemy::birdScale(0.12f, 0.12f);
const vec2 Enemy::boulderScale(0.38f, 0.38f);

Enemy::Enemy(GameObject *gameObject) : Entity(gameObject) {
}

// ====================== ENEMY =======================
// ================ by: Eimantas Urbutis 11.11.18 ======================

void Enemy::initEnemy(std::shared_ptr<SpriteAtlas> enemyAtlas, vec2 position, EnemyType enemyType) {
    //auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    //auto physicsScale = EmeraldGame::gameInstance->physicsScale;
    //physicsComponent = gameObject->addComponent<PhysicsComponent>();
    enemyVelocity = 0.25f;
    fireOffset = (rand() % 20) + 10;
    //physicsComponent->getFixture()->SetRestitution(1);
    this->enemyType = enemyType;
    int enemySprite = (rand() % 3) + 1;
    if (enemyType == EnemyType::Zombie) {
        radius = 10 / physicsScale;
        physicsComponent->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        physicsComponent->setLinearVelocity({0.1, 0});
        enemySprite = (rand() % 2) + 1;
        auto zombieSpriteObj = enemyAtlas->get("walking" + to_string(enemySprite) + "_1.png");
        zombieSpriteObj.setScale(birdScale);
        spriteComponent->setSprite(zombieSpriteObj);
        setSprites(enemyAtlas->get("walking" + to_string(enemySprite) + "_1.png"),
                   enemyAtlas->get("walking" + to_string(enemySprite) + "_2.png"),
                   enemyAtlas->get("walking" + to_string(enemySprite) + "_1.png"),
                   enemyAtlas->get("walking" + to_string(enemySprite) + "_2.png"),
                   enemyAtlas->get("walkingHit" + to_string(enemySprite) + "_1.png"),
                   enemyAtlas->get("walkingHit" + to_string(enemySprite) + "_1.png"),
                   birdScale);
    } else if (enemyType == EnemyType::AngryBird) {
        radius = 10 / physicsScale;
        physicsComponent->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        physicsComponent->setLinearVelocity({0.1, 0});
        flyingEnemy = true;
        auto birdSpriteObj = enemyAtlas->get("jumping" + to_string(enemySprite) + "_1.png");
        birdSpriteObj.setScale(birdScale);
        spriteComponent->setSprite(birdSpriteObj);
        setSprites(enemyAtlas->get("jumping" + to_string(enemySprite) + "_1.png"),
                   enemyAtlas->get("jumping" + to_string(enemySprite) + "_2.png"),
                   enemyAtlas->get("jumping" + to_string(enemySprite) + "_3.png"),
                   enemyAtlas->get("jumping" + to_string(enemySprite) + "_4.png"),
                   enemyAtlas->get("jumpingHit" + to_string(enemySprite) + "_1.png"),
                   enemyAtlas->get("jumpingHit" + to_string(enemySprite) + "_1.png"),
                   birdScale);
    } else if (enemyType == EnemyType::Dragon) {
        radius = 10 / physicsScale;
        physicsComponent->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        physicsComponent->setLinearVelocity({1.5, 0});
        auto dragonSpriteObj = enemyAtlas->get("flying" + to_string(enemySprite) + "_1.png");
        dragonSpriteObj.setScale(dragonScale);
        flyingEnemy = true;
        spriteComponent->setSprite(dragonSpriteObj);
        setSprites(enemyAtlas->get("flying" + to_string(enemySprite) + "_1.png"),
                   enemyAtlas->get("flying" + to_string(enemySprite) + "_2.png"),
                   enemyAtlas->get("flying" + to_string(enemySprite) + "_3.png"),
                   enemyAtlas->get("flying" + to_string(enemySprite) + "_4.png"),
                   enemyAtlas->get("flyingHit" + to_string(enemySprite) + "_1.png"),
                   enemyAtlas->get("flyingHit" + to_string(enemySprite) + "_1.png"),
                   dragonScale);
    } else if (enemyType == EnemyType::Poison) {
        gameObject->name = "Poison";
        radius = 5 / physicsScale;
        enemyVelocity = 0.0f;
        physicsComponent->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        auto spikeSpriteObj = enemyAtlas->get("poison_bottle.png");
        spikeSpriteObj.setScale(birdScale);
        spriteComponent->setSprite(spikeSpriteObj);
        setSprites(enemyAtlas->get("poison_bottle.png"),
                   enemyAtlas->get("poison_bottle.png"),
                   enemyAtlas->get("poison_bottle.png"),
                   enemyAtlas->get("poison_bottle.png"),
                   enemyAtlas->get("poison_bottle.png"),
                   enemyAtlas->get("poison_bottle.png"),
                   birdScale);
    } else {
        gameObject->name = "Boulder";
        radius = 10 / physicsScale;
        physicsComponent->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        physicsComponent->setLinearVelocity({1, 0});
        enemySprite = (rand() % 2) + 1;
        auto boulderSpriteObj = enemyAtlas->get("boulder_" + to_string(enemySprite) + ".png");
        boulderSpriteObj.setScale(dragonScale);
        flyingEnemy = false;
        spriteComponent->setSprite(boulderSpriteObj);
        setSprites(enemyAtlas->get("boulder_" + to_string(enemySprite) + ".png"),
                   enemyAtlas->get("boulder_" + to_string(enemySprite) + ".png"),
                   enemyAtlas->get("boulder_" + to_string(enemySprite) + ".png"),
                   enemyAtlas->get("boulder_" + to_string(enemySprite) + ".png"),
                   enemyAtlas->get("boulder_" + to_string(enemySprite) + ".png"),
                   enemyAtlas->get("boulder_" + to_string(enemySprite) + ".png"),
                   dragonScale);
        enemyVelocity = 25.0f;
        vec2 movement{0, 0};
        if (facingLeft) {
            movement.x = movement.x - enemyVelocity;
        } else {
            movement.x = movement.x + enemyVelocity;
        }
        float accelerationSpeed = 0.01f;
        physicsComponent->addImpulse(movement * accelerationSpeed);

        pos = physicsComponent->getPosition();
        physicsComponent->getFixture()->SetFriction(0.25);
        physicsComponent->fixRotation();
        //physicsComponent->getBody()->SetTransform();
        //physicsComponent->getBody()->ApplyAngularImpulse(5, true);
        physicsComponent->getFixture()->SetRestitution(0.4);
    }
    if (!(enemyType == EnemyType::Boulder)) {
        pos = physicsComponent->getPosition();
        physicsComponent->fixRotation();
        physicsComponent->getFixture()->SetFriction(1);
    }
}

// ================================ Shooting system ======================
// =========================== by: Eimantas Urbutis ======================
void Enemy::dropBoulder() {
    auto game = EmeraldGame::gameInstance;
    game->deleteGameObject(cannonObject);
    EmeraldGame::gameInstance->audioManager->playSFX("cannon.mp3");
    cannonObject = game->createGameObject();
    auto spriteComponent = cannonObject->addComponent<SpriteComponent>();
    auto physicsScale = EmeraldGame::gameInstance->physicsScale;
    auto cannonPhysics = cannonObject->addComponent<PhysicsComponent>();
    auto cannonRadius = 10 / physicsScale;
    auto cannonPos = gameObject->getPosition() / physicsScale;
    cannonPhysics->initCircle(b2_dynamicBody, cannonRadius, cannonPos, 1);
    if (enemyType == EnemyType::Dragon) {
        cannonObject->name = "Boulder";
        cannonPhysics->setSensor(true);
        int enemySprite = (rand() % 2) + 1;
        auto boulderSpriteObj = EmeraldGame::gameInstance->obstaclesAtlas->get("boulder_" + to_string(enemySprite) + ".png");
        boulderSpriteObj.setScale(boulderScale);
        spriteComponent->setSprite(boulderSpriteObj);
    } else {
        cannonObject->name = "Poison";
        auto poisonSpriteObj = EmeraldGame::gameInstance->obstaclesAtlas->get("poison_bottle.png");
        poisonSpriteObj.setScale(dragonScale);
        spriteComponent->setSprite(poisonSpriteObj);
    }
    fireTimer = 0.0;
}


void Enemy::update(float deltaTime) {
    // raycast ignores any shape in the starting point
    auto from = physicsComponent->getBody()->GetWorldCenter();
    b2Vec2 to{from.x, from.y - radius * 1.3f};

    isGrounded = false;
    EmeraldGame::gameInstance->world->RayCast(this, from, to);

    // To keep dragon at same height while ignoring gravity

    if (enemyType == EnemyType::Dragon && !isDead) {
        physicsComponent->addForce({physicsComponent->getLinearVelocity().x, 0});
        physicsComponent->moveTo({physicsComponent->getPosition().x, pos.y});
    } else if (enemyType == EnemyType::Zombie || (enemyType == EnemyType::AngryBird)) {
        physicsComponent->addForce({physicsComponent->getLinearVelocity().x, 0});
    }

    vec2 movement{0, 0};
    if (facingLeft) {
        movement.x = movement.x - enemyVelocity;
    } else {
        movement.x = movement.x + enemyVelocity;
    }

    b2Body *body = physicsComponent->getBody();

    // ====================== Enemy VELOCITY =====================
    if (!(enemyType == EnemyType::Boulder)) {
        float accelerationSpeed = 0.01f;
        physicsComponent->addImpulse(movement * accelerationSpeed);
        float maximumVelocity = enemyVelocity;
        auto linearVelocity = physicsComponent->getLinearVelocity();
        float currentVelocity = linearVelocity.x;
        if (abs(currentVelocity) > maximumVelocity) {
            linearVelocity.x = sign(linearVelocity.x) * maximumVelocity;
            physicsComponent->setLinearVelocity(linearVelocity);
        }
        updateSprite(deltaTime);
    } else {
        rotation -= physicsComponent->getBody()->GetLinearVelocity().x;
        gameObject->setRotation(rotation);
    }
    // ====================== Enemy DIES =======================
    if (!isDead)
        fireTimer += deltaTime;
    if (fireTimer > fireOffset) {
        dropBoulder();
    }
}

void Enemy::jump() {
    auto world = EmeraldGame::gameInstance->world;
    if (world->GetGravity().y < 0) {
        physicsComponent->addImpulse({0, 0.1f});
    } else {
        physicsComponent->addImpulse({0, -0.1f});
    }

}

// On collision with any object enemy turns around
void Enemy::onCollisionStart(PhysicsComponent *comp) {
    if (comp->getGameObject()->name == "Wall" && enemyType != EnemyType::Dragon) {
        Sprite sprite = gameObject->getComponent<SpriteComponent>()->getSprite();
        sprite.setFlip({true, false});
        gameObject->getComponent<SpriteComponent>()->setSprite(sprite);
        facingLeft = !facingLeft;
    } else if (comp->getGameObject()->name == "Cannon") {
        isDead = true;
    }
    if (!isDead) {
        if (enemyType == EnemyType::AngryBird) {
            this->jump();
        }
        if (enemyType == EnemyType::Dragon && comp->getGameObject()->name != "Boulder") {
            auto linearVelocity = physicsComponent->getLinearVelocity();
            physicsComponent->setLinearVelocity({linearVelocity.x * -1, linearVelocity.y});
            facingLeft = !facingLeft;
        }
    }
}

void Enemy::onCollisionEnd(PhysicsComponent *comp) {

}

float32
Enemy::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    isGrounded = true;
    return 0; // terminate raycast
}

void Enemy::setSprites(Sprite move1, Sprite move2, Sprite move3, Sprite move4,
                       Sprite dead1, Sprite dead2, vec2 scale) {
    move1.setScale(scale);
    move2.setScale(scale);
    move3.setScale(scale);
    move4.setScale(scale);
    dead1.setScale(scale);
    dead2.setScale(scale);
    this->move1 = move1;
    deadSprites = {dead1, dead2};
    movingSprites = {move1, move2, move3, move4};
}

// ====================== ENEMY ANIMATIONS =======================

void Enemy::updateSprite(float deltaTime) {
    auto velocity = physicsComponent->getLinearVelocity();
    distance += velocity.x * deltaTime;
    if (velocity.x == 0.0f) {
        move1.setFlip({(facingLeft), false});
        gameObject->getComponent<SpriteComponent>()->setSprite(move1);
    } else if ((distance > 0.02 && !isDead) || (distance < -0.02 && !isDead)) {
        spriteIndex = (spriteIndex + 1) % movingSprites.size();
        Sprite movingSprite = movingSprites[spriteIndex];
        if (distance > 0.02)
            movingSprite.setFlip({true, false});
        gameObject->getComponent<SpriteComponent>()->setSprite(movingSprite);
        distance = 0.0f;
    }
    if (isDead) {
        // TODO: change sprites rotation logic
        spriteIndex = (spriteIndex + 1) % deadSprites.size();
        Sprite deadSprite = deadSprites[spriteIndex];
        gameObject->getComponent<SpriteComponent>()->setSprite(deadSprite);
    }

    // Flips sprite for gravity room
    auto world = EmeraldGame::gameInstance->world;
    if (world->GetGravity().y > 0) {
        auto lastSprite = gameObject->getComponent<SpriteComponent>()->getSprite();
        lastSprite.setFlip({false, true});
        gameObject->getComponent<SpriteComponent>()->setSprite(lastSprite);
    }
}
