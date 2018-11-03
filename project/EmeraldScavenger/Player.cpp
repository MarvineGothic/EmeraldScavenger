//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "Player.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "EmeraldGame.hpp"
#include "SpriteComponent.hpp"

Player::Player(GameObject *gameObject) : Component(gameObject) {
    characterPhysics = gameObject->addComponent<PhysicsComponent>();

    auto physicsScale = EmeraldGame::gameInstance->physicsScale;
    radius = 10 / physicsScale;
    characterPhysics->initCircle(b2_dynamicBody, radius, vec2{1.5, 1.5} * Level::tileSize / physicsScale, 1);
    //characterPhysics->getFixture()->SetRestitution(1);
    characterPhysics->fixRotation();
    characterPhysics->getFixture()->SetFriction(1);
    spriteComponent = gameObject->getComponent<SpriteComponent>();

}

bool Player::onKey(SDL_Event &event) {

    switch (event.key.keysym.sym) {
        case SDLK_SPACE: {
            if (isGrounded && event.type == SDL_KEYDOWN) { // prevents double jump
                jump();
            }
        }
            break;
        case SDLK_LEFT: {
            left = event.type == SDL_KEYDOWN;
        }
            break;
        case SDLK_RIGHT: {
            right = event.type == SDL_KEYDOWN;
        }
            break;
        default:
            break;
    }

    return false;
}

void Player::update(float deltaTime) {
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

    // ====================== PLAYER VELOCITY =====================
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

    // ====================== PLAYER DIES =======================
    if (this->getGameObject()->getPosition().y <= 0) {
        EmeraldGame::gameInstance->livesCounter--;
        auto lives = EmeraldGame::gameInstance->livesCounter;
        EmeraldGame::gameInstance->setGameState(GameState::GetReady);
        cout << "Loose life " << lives << endl;
        isDead = true;
    }

}

void Player::jump() {
    characterPhysics->addImpulse({0, 0.15f});
}

void Player::onCollisionStart(PhysicsComponent *comp) {

}

void Player::onCollisionEnd(PhysicsComponent *comp) {

}

float32
Player::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    isGrounded = true;
    return 0; // terminate raycast
}

void Player::setSprites(sre::Sprite standing, sre::Sprite walk1, sre::Sprite walk2, sre::Sprite flyUp,
                        sre::Sprite fly, sre::Sprite flyDown) {
    this->standing = standing;
    this->walkLeft = walk1;
    this->walkRight = walk2;
    this->flyUp = flyUp;
    this->fly = fly;
    this->flyDown = flyDown;
}

void Player::updateSprite(float deltaTime) {
    auto velocity = characterPhysics->getLinearVelocity();
    // todo: animation
}


