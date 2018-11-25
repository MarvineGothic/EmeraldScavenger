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

const vec2 Enemy::dragonScale(0.3f, 0.3f);
const vec2 Enemy::birdScale(0.5f, 0.5f);

Enemy::Enemy(GameObject *gameObject) : Component(gameObject) {
}

// ====================== ENEMY =======================
// ================ by: Eimantas Urbutis 11.11.18 ======================

void Enemy::initEnemy(std::shared_ptr<sre::SpriteAtlas> enemyAtlas, vec2 position, EnemyType enemyType) {
    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto physicsScale = EmeraldGame::gameInstance->physicsScale;
    characterPhysics = gameObject->addComponent<PhysicsComponent>();
    enemyVelocity = 0.25f;
    
    //characterPhysics->getFixture()->SetRestitution(1);
    this->enemyType=enemyType;

    if (enemyType == EnemyType::Zombie) {
        radius = 10 / physicsScale;
        characterPhysics->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        auto zombieSpriteObj = enemyAtlas->get("frame-1_zombie_idle.png");
        zombieSpriteObj.setScale(EmeraldGame::scale);
        spriteComponent->setSprite(zombieSpriteObj);
        setSprites(enemyAtlas->get("frame-1_zombie_idle.png"),
                   enemyAtlas->get("frame-2_zombie_idle.png"),
                   enemyAtlas->get("frame-1_zombie_idle.png"),
                   enemyAtlas->get("frame-2_zombie_idle.png"),
                   enemyAtlas->get("frame-1_zombie_hit.png"),
                   enemyAtlas->get("frame-2_zombie_hit.png"),
                   EmeraldGame::scale);
    } else if (enemyType == EnemyType::AngryBird) {
        radius = 10 / physicsScale;
        characterPhysics->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        flyingEnemy = true;
        auto birdSpriteObj = enemyAtlas->get("1.png");
        birdSpriteObj.setScale(birdScale);
        spriteComponent->setSprite(birdSpriteObj);
        setSprites(enemyAtlas->get("1.png"),
                   enemyAtlas->get("2.png"),
                   enemyAtlas->get("3.png"),
                   enemyAtlas->get("4.png"),
                   enemyAtlas->get("1.png"),
                   enemyAtlas->get("2.png"),
                   birdScale);
    } else {
        radius = 10 / physicsScale;
        characterPhysics->initCircle(b2_dynamicBody, radius, position * Level::tileSize / physicsScale, 1);
        characterPhysics->setLinearVelocity({1,0});
        auto dragonSpriteObj = enemyAtlas->get("frame-1_dragon.png");
        dragonSpriteObj.setScale(dragonScale);
        flyingEnemy = true;
        spriteComponent->setSprite(dragonSpriteObj);
        setSprites(enemyAtlas->get("frame-1_dragon.png"),
                   enemyAtlas->get("frame-2_dragon.png"),
                   enemyAtlas->get("frame-3_dragon.png"),
                   enemyAtlas->get("frame-4_dragon.png"),
                   enemyAtlas->get("frame-1_dragon.png"),
                   enemyAtlas->get("frame-2_dragon.png"),
                   dragonScale);
    }
    pos = characterPhysics->getPosition();
    characterPhysics->fixRotation();
    characterPhysics->getFixture()->SetFriction(1);
}


bool Enemy::onKey(SDL_Event &event) {
    return false;
}

void Enemy::update(float deltaTime) {
    // raycast ignores any shape in the starting point
    auto from = characterPhysics->getBody()->GetWorldCenter();
    b2Vec2 to{from.x, from.y - radius * 1.3f};
    
    isGrounded = false;
    EmeraldGame::gameInstance->world->RayCast(this, from, to);
    
    // To keep dragon at same height while ignoring gravit
    if (enemyType==EnemyType::Dragon) {
        characterPhysics->addForce({characterPhysics->getLinearVelocity().x, 0});
        characterPhysics->moveTo({characterPhysics->getPosition().x, pos.y});
    }

    vec2 movement{0, 0};
    if (facingLeft) {
        movement.x = movement.x - enemyVelocity;
    } else {
        movement.x = movement.x + enemyVelocity;
    }

    b2Body *body = characterPhysics->getBody();

    // ====================== Enemy VELOCITY =====================
    float accelerationSpeed = 0.01f;
    characterPhysics->addImpulse(movement * accelerationSpeed);
    float maximumVelocity = enemyVelocity;
    auto linearVelocity = characterPhysics->getLinearVelocity();
    float currentVelocity = linearVelocity.x;
    if (abs(currentVelocity) > maximumVelocity) {
        linearVelocity.x = sign(linearVelocity.x) * maximumVelocity;
        characterPhysics->setLinearVelocity(linearVelocity);
    }

    updateSprite(deltaTime);

    // ====================== Enemy DIES =======================
    //TODO

}

void Enemy::jump() {
    characterPhysics->addImpulse({0, 0.25f});
}

// On collision with any object enemy turns around
void Enemy::onCollisionStart(PhysicsComponent *comp) {
    if (comp->getGameObject()->name == "Wall" && enemyType!=EnemyType::Dragon) {
        Sprite sprite = gameObject->getComponent<SpriteComponent>()->getSprite();
        sprite.setFlip({true, false});
        gameObject->getComponent<SpriteComponent>()->setSprite(sprite);
        facingLeft = !facingLeft;
    }
    if (enemyType==EnemyType::AngryBird) {
        this->jump();
    }
    if (enemyType==EnemyType::Dragon) {
        auto linearVelocity = characterPhysics->getLinearVelocity();
        characterPhysics->setLinearVelocity({linearVelocity.x*-1,linearVelocity.y});
        facingLeft = !facingLeft;
    }
}

void Enemy::onCollisionEnd(PhysicsComponent *comp) {

}

float32
Enemy::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    isGrounded = true;
    return 0; // terminate raycast
}

void Enemy::setSprites(sre::Sprite move1, sre::Sprite move2, sre::Sprite move3, sre::Sprite move4,
                       sre::Sprite dead1, sre::Sprite dead2, vec2 scale) {
    move1.setScale(scale);
    move2.setScale(scale);
    move3.setScale(scale);
    move4.setScale(scale);
    dead1.setScale(scale);
    dead2.setScale(scale);
    this->move1 = move1;
    this->move2 = move2;
    this->move3 = move3;
    this->move4 = move4;
    this->dead1 = dead1;
    this->dead2 = dead2;
    deadSprites = {dead1, dead2};
    movingSprites = {move1, move2, move3, move4};
}

// ====================== ENEMY ANIMATIONS =======================

void Enemy::updateSprite(float deltaTime) {
    auto velocity = characterPhysics->getLinearVelocity();
    distance += velocity.x * deltaTime;
    if (velocity.x == 0.0f) {
        move1.setFlip({(facingLeft), false});
        gameObject->getComponent<SpriteComponent>()->setSprite(move1);
    } else if (distance > 0.02 || distance < -0.02) {
        spriteIndex = (spriteIndex + 1) % movingSprites.size();
        Sprite movingSprite = movingSprites[spriteIndex];
        if (distance > 0.02 && enemyType!=EnemyType::Dragon)
            movingSprite.setFlip({true, false});
        else if (distance < -0.02 && enemyType==EnemyType::Dragon)
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
}
