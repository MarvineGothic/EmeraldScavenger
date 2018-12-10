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
#include "Door.hpp"

Player::Player(GameObject *gameObject) : Component(gameObject) {
    characterPhysics = gameObject->addComponent<PhysicsComponent>();
    accelerationSpeed = 1.00f;
    maximumVelocity = 2.0f;
    physicsScale = EmeraldGame::gameInstance->physicsScale;
    radius = 16 / physicsScale;
    characterPhysics->initCircle(b2_dynamicBody, radius, Level::getStartPos() / physicsScale, 1);
    characterPhysics->fixRotation();
    characterPhysics->getFixture()->SetFriction(1);
    characterPhysics->getFixture()->SetDensity(0.1f);
    spriteComponent = gameObject->getComponent<SpriteComponent>();
    lastSprite = idle;
}

bool Player::onKey(SDL_Event &event) {

    switch (event.key.keysym.sym) {
        case SDLK_SPACE: {
            if (isGrounded && event.type == SDL_KEYDOWN && !spaceKey) {
                isJump = true;
                spaceKey = true;
            }
            if (event.type == SDL_KEYUP) {
                spaceKey = false;
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
    if (blinkTime <= 2.5f) {
        if (left) {
            movement.x--;
            facingLeft = true;
        }

        if (right) {
            movement.x++;
            facingLeft = false;
        }
        if (isJump)jump();
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

    // ====================== PLAYER FALLS IN THE PIT ===================
    // ================ by: Sergiy Isakov 02.11.18 ======================
    if (this->getGameObject()->getPosition().y <= 0) {
        inPit = true;
        lostLife = true;
        EmeraldGame::gameInstance->audioManager->playSFX("inPit.wav", 8);
    }

}

void Player::jump() {
    EmeraldGame::gameInstance->audioManager->playSFX("jump.wav", 8);
    isJump = false;
    auto world = EmeraldGame::gameInstance->world;
    if (world->GetGravity().y < 0) {
        characterPhysics->addImpulse({0, 0.4f});
    } else {
        characterPhysics->addImpulse({0, -0.4f});
    }
}

void Player::onCollisionStart(PhysicsComponent *comp) {
    // ================================ PLAYER COLLISIONS ================================
    // ================ by: Sergiy Isakov 17.11.18 22:16 =================================
    auto obj = comp->getGameObject();
    if (!invincible && obj->name == "Enemy") {
        lostLife = true;
        blinkTime = 3.0f;
        vec2 pV = characterPhysics->getLinearVelocity();
        characterPhysics->addImpulse({-pV.x, -pV.y * 0.1f});
        EmeraldGame::gameInstance->audioManager->playSFX("lostLife.wav", 8);
    }
    if (obj->name == "Emerald" && obj->getComponent<SpriteComponent>() != nullptr) {
        EmeraldGame::gameInstance->level->deleteEmerald(obj->getComponent<CollectibleItem>());
        obj->removeComponent(obj->getComponent<SpriteComponent>());
        if (EmeraldGame::gameInstance->emeraldCounter < Level::getEmeraldsNeeded())
            EmeraldGame::gameInstance->emeraldCounter++;
        EmeraldGame::gameInstance->audioManager->playSFX("emeraldPickUp.wav");
    }
    if (obj->name == "Pie" && obj->getComponent<SpriteComponent>() != nullptr) {
        EmeraldGame::gameInstance->level->deleteEmerald(obj->getComponent<CollectibleItem>());
        obj->removeComponent(obj->getComponent<SpriteComponent>());
        if (EmeraldGame::gameInstance->livesCounter < 5) {
            EmeraldGame::gameInstance->livesCounter++;
        }
        EmeraldGame::gameInstance->audioManager->playSFX("lifeUp.wav");
    }
    if (obj->name == "Door" && obj->getComponent<Door>()->isExit) {
        exit = true;

		EmeraldGame::nextLevel = obj->getComponent<Door>()->level;
		EmeraldGame::nextStartPosition = obj->getComponent<Door>()->nextLevelStartPosition;

    }
}

void Player::onCollisionEnd(PhysicsComponent *comp) {
    auto obj = comp->getGameObject();
    if (obj->name == "Door" && obj->getComponent<Door>()->isExit) {
        exit = false;
    }
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
    this->death = death;
    runningSprites = {run1, run2, run3};
}

void Player::updateSprite(float deltaTime) {

    // ========================== PLAYER HIT ANIMATION ==================================
    // ========================== by: Sergiy Isakov 17.11.18 17:32 ======================

    if (invincible && blinkTime <= 2.5f) {
        this->gameObject->removeComponent(spriteComponent);
        blinkDelta += deltaTime;
        if (blinkDelta >= blinkFreq) {
            gameObject->addComponent<SpriteComponent>();
            spriteComponent = gameObject->getComponent<SpriteComponent>();
            blinkTime -= blinkDelta;
            blinkDelta = 0.0f;
        }
        if (blinkTime <= 0.0f && gameObject->getComponent<SpriteComponent>() == nullptr)
            invincible = false;
    }
    if (!invincible && gameObject->getComponent<SpriteComponent>() == nullptr) {
        gameObject->addComponent<SpriteComponent>();
        spriteComponent = gameObject->getComponent<SpriteComponent>();
    }


    auto velocity = characterPhysics->getLinearVelocity();
    // ================================ PLAYER SPRITES ANIMATION ==============================
    // =========================== by: Sergiy Isakov 05.11.18 05:07 ======================
    auto world = EmeraldGame::gameInstance->world;
    if (world->GetGravity().y > 5 || velocity.y * deltaTime == posY) {
        isGrounded= true;
    }
    posY = velocity.y * deltaTime;
    
    if (isGrounded) {
        distance += velocity.x * deltaTime;
        if (velocity.x == 0.0f) {
            idle.setFlip({(facingLeft), false});
            lastSprite = idle;
        }
        if (distance > 0.06 || distance < -0.06) {
            spriteIndex = (spriteIndex + 1) % runningSprites.size();
            Sprite runningSprite = runningSprites[spriteIndex];
            if (distance < -0.06)
                runningSprite.setFlip({true, false});
            distance = 0.0f;
            lastSprite = runningSprite;
        }
    } else {
        if (velocity.y > 0) {
            jumpUp.setFlip({(facingLeft), false});
            lastSprite = jumpUp;
        }
        if (velocity.y < 0) {
            fall.setFlip({(facingLeft), false});
            lastSprite = fall;
        }
    }
    if (blinkTime > 2.5f) {
        death.setFlip({(facingLeft), false});
        lastSprite = death;
        invincible = true;
        blinkTime -= deltaTime;
    }

    spriteComponent->setSprite(lastSprite);
    // Flips sprite for gravity room
    if (world->GetGravity().y > 0) {
        lastSprite.setFlip({false, true});
        if (facingLeft)
            lastSprite.setFlip({true, true});
        spriteComponent->setSprite(lastSprite);
    }
}
