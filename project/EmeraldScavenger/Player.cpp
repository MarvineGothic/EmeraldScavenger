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
    accelerationSpeed = 1.00f;
    maximumVelocity = 2.0f;

    auto physicsScale = EmeraldGame::gameInstance->physicsScale;
    radius = 16 / physicsScale;
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
        facingLeft = true;
    }

    if (right) {
        movement.x++;
        facingLeft = false;
    }

    // ====================== PLAYER VELOCITY =====================

    characterPhysics->addImpulse(movement * accelerationSpeed);
    auto linearVelocity = characterPhysics->getLinearVelocity();
    float currentVelocity = linearVelocity.x;
    if (abs(currentVelocity) > maximumVelocity) {
        linearVelocity.x = sign(linearVelocity.x) * maximumVelocity;
        characterPhysics->setLinearVelocity(linearVelocity);
    }

    updateSprite(deltaTime);

    // ====================== PLAYER DIES =======================
    // ================ by: Sergiy Isakov 02.11.18 ======================
    if (this->getGameObject()->getPosition().y <= 0) {
        EmeraldGame::gameInstance->livesCounter--;
        EmeraldGame::gameInstance->setGameState(GameState::GetReady);
        isDead = true;
    }

}

void Player::jump() {
    characterPhysics->addImpulse({0, 0.4f});
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

void Player::setSprites(Sprite idle, Sprite jumpUp, Sprite fall, Sprite run1,
                        Sprite run2, Sprite run3, Sprite death) {
    auto scale = EmeraldGame::scale;
    idle.setScale(scale);
    jumpUp.setScale(scale);
    fall.setScale(scale);
    run1.setScale(scale);
    run2.setScale(scale);
    run3.setScale(scale);
    death.setScale(scale);
    this->idle = idle;
    this->jumpUp = jumpUp;
    this->fall = fall;
    this->run1 = run1;
    this->run2 = run2;
    this->run3 = run3;
    this->death = death;
    runningSprites = {run1, run2, run3};
}

void Player::updateSprite(float deltaTime) {
    auto velocity = characterPhysics->getLinearVelocity();
    // ================================ animation ==============================
    // ============= by: Sergiy Isakov 05.11.18 05:07 ======================
    if (isGrounded) {
        distance += velocity.x * deltaTime;
        if (velocity.x == 0.0f) {
            idle.setFlip({(facingLeft), false});
            spriteComponent->setSprite(idle);
        }
        if (distance > 0.06 || distance < -0.06) {
            spriteIndex = (spriteIndex + 1) % runningSprites.size();
            Sprite runningSprite = runningSprites[spriteIndex];
            if (distance < -0.06)
                runningSprite.setFlip({true, false});
            spriteComponent->setSprite(runningSprite);
            distance = 0.0f;
        }
    } else if (velocity.y > 0) {
        jumpUp.setFlip({(facingLeft), false});
        spriteComponent->setSprite(jumpUp);
    } else if (velocity.y < 0) {
        fall.setFlip({(facingLeft), false});
        spriteComponent->setSprite(fall);
    }
    if (isDead) {
        death.setFlip({(facingLeft), false});
        spriteComponent->setSprite(death);
    }
}


