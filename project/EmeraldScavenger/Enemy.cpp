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

Enemy::Enemy(GameObject *gameObject) : Component(gameObject) {
    characterPhysics = gameObject->addComponent<PhysicsComponent>();

    auto physicsScale = EmeraldGame::gameInstance->physicsScale;
    radius = 16 / physicsScale;
    characterPhysics->initCircle(b2_dynamicBody, radius, vec2{1.5, 1.5} * Level::tileSize / physicsScale, 1);
    //characterPhysics->getFixture()->SetRestitution(1);
    characterPhysics->fixRotation();
    characterPhysics->getFixture()->SetFriction(1);
    spriteComponent = gameObject->getComponent<SpriteComponent>();

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

    vec2 movement{0, 0};
    if (left) {
        movement.x--;
    }

    if (right) {
        movement.x++;
    }

    b2Body *body = characterPhysics->getBody();

    // ====================== Enemy VELOCITY =====================
    float accelerationSpeed = 0.01f;
    characterPhysics->addImpulse(movement * accelerationSpeed);
    float maximumVelocity = 2;
    auto linearVelocity = characterPhysics->getLinearVelocity();
    float currentVelocity = linearVelocity.x;
    if (abs(currentVelocity) > maximumVelocity) {
        linearVelocity.x = sign(linearVelocity.x) * maximumVelocity;
        characterPhysics->setLinearVelocity(linearVelocity);
    }

    updateSprite(deltaTime);

    // ====================== Enemy DIES =======================

}

void Enemy::jump() {
    characterPhysics->addImpulse({0, 0.4f});
}

void Enemy::onCollisionStart(PhysicsComponent *comp) {

}

void Enemy::onCollisionEnd(PhysicsComponent *comp) {

}

float32
Enemy::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    isGrounded = true;
    return 0; // terminate raycast
}

void Enemy::setSprites(sre::Sprite idle, sre::Sprite jump1, sre::Sprite jump2, sre::Sprite run1,
                       sre::Sprite run2, sre::Sprite run3, sre::Sprite death) {
    this->idle = idle;
    this->jump1 = jump1;
    this->jump2 = jump2;
    this->run1 = run1;
    this->run2 = run2;
    this->run3 = run3;
    this->death = death;
}

void Enemy::updateSprite(float deltaTime) {
    auto velocity = characterPhysics->getLinearVelocity();
    // todo: animation
}