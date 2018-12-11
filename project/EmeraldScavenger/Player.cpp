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

Player::Player(GameObject *gameObject) : Entity(gameObject) {
    accelerationSpeed = 1.00f;
    maximumVelocity = 2.0f;
    radius = 16 / physicsScale;
    physicsComponent->initCircle(b2_dynamicBody, radius, Level::getStartPos() / physicsScale, 1);
    physicsComponent->fixRotation();
    physicsComponent->getFixture()->SetFriction(1);
    physicsComponent->getFixture()->SetDensity(0.1f);
    auto playerSpriteObj = EmeraldGame::gameInstance->scavengerAtlas->get("boy-idle.png");
    playerSpriteObj.setScale(EmeraldGame::scale);
    spriteComponent->setSprite(playerSpriteObj);
    entitySprite = idle;
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
    auto from = physicsComponent->getBody()->GetWorldCenter();
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

        fireTimer += deltaTime;
    }
    // ====================== PLAYER VELOCITY =====================

    physicsComponent->addImpulse(movement * accelerationSpeed);
    auto linearVelocity = physicsComponent->getLinearVelocity();
    float currentVelocity = linearVelocity.x;
    if (abs(currentVelocity) > maximumVelocity) {
        linearVelocity.x = sign(linearVelocity.x) * maximumVelocity;
        physicsComponent->setLinearVelocity(linearVelocity);
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
        physicsComponent->addImpulse({0, 0.4f});
    } else {
        physicsComponent->addImpulse({0, -0.4f});
    }
}

void Player::onCollisionStart(PhysicsComponent *comp) {
    // ================================ PLAYER COLLISIONS ================================
    // ================ by: Sergiy Isakov 17.11.18 22:16 =================================
    auto obj = comp->getGameObject();
    if (!invincible && obj->name == "Enemy") {
        if (!obj->getComponent<Enemy>()->isDead) {
            lostLife = true;
            blinkTime = 3.0f;
            vec2 pV = physicsComponent->getLinearVelocity();
            physicsComponent->addImpulse({-pV.x, -pV.y * 0.1f});
            EmeraldGame::gameInstance->audioManager->playSFX("lostLife.wav", 8);
        }
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
    if (obj->name == "Platform") {
        touchesPlatform = true;
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
    if (obj->name == "Platform") {
        touchesPlatform = false;
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

// ================================ Shooting system ======================
// =========================== by: Eimantas Urbutis ======================
void Player::fireCannon(std::shared_ptr<sre::SpriteAtlas> spritesAtlas) {
    if (fireTimer > 2.5) {
        EmeraldGame::gameInstance->audioManager->playSFX("cannon.mp3");
        auto game = EmeraldGame::gameInstance;
        game->deleteGameObjectsByName("Cannon");
        auto cannonObject = game->createGameObject();
        cannonObject->name = "Cannon";
        auto spriteComponent = cannonObject->addComponent<SpriteComponent>();
        auto physicsScale = EmeraldGame::gameInstance->physicsScale;
        auto cannonPhysics = cannonObject->addComponent<PhysicsComponent>();
        auto cannonVelocity = 3.0f;
        auto cannonRadius = 5 / physicsScale;

        auto cannonPos = gameObject->getPosition() / physicsScale;
        if (facingLeft) {
            cannonPos.x -= 0.1;
        } else {
            cannonPos.x += 0.1;
        }
        cannonPhysics->initCircle(b2_dynamicBody, cannonRadius, cannonPos, 1);
        //    cannonPhysics->setSensor(true);
        auto cannonSpriteObj = spritesAtlas->get("diamond blue.png");
        cannonSpriteObj.setColor({0.0, 1.0, 0.0, 1.0});
        cannonSpriteObj.setScale({0.05f, 0.05f});
        spriteComponent->setSprite(cannonSpriteObj);
        vec2 movement{0, 0};
        if (facingLeft) {
            cannonPhysics->setLinearVelocity({-1, 0});
            movement.x = movement.x - cannonVelocity;
        } else {
            cannonPhysics->setLinearVelocity({1, 0});
            movement.x = movement.x + cannonVelocity;
        }
        float accelerationSpeed = 0.01f;
        cannonPhysics->addImpulse(movement * accelerationSpeed);
        cannonPhysics->getFixture()->SetFriction(0.25);
        cannonPhysics->getFixture()->SetRestitution(0.4);
        fireTimer = 0.0;
    }
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


    auto velocity = physicsComponent->getLinearVelocity();
    // ================================ PLAYER SPRITES ANIMATION ==============================
    // =========================== by: Sergiy Isakov 05.11.18 05:07 ======================
    auto world = EmeraldGame::gameInstance->world;
    if (touchesPlatform) {
        isGrounded = true;
    }

    if (isGrounded) {
        distance += velocity.x * deltaTime;
        if (velocity.x == 0.0f) {
            idle.setFlip({(facingLeft), false});
            entitySprite = idle;
        }
        if (distance > 0.06 || distance < -0.06) {
            spriteIndex = (spriteIndex + 1) % runningSprites.size();
            Sprite runningSprite = runningSprites[spriteIndex];
            if (distance < -0.06)
                runningSprite.setFlip({true, false});
            distance = 0.0f;
            entitySprite = runningSprite;
        }
    } else {
        if (velocity.y > 0) {
            jumpUp.setFlip({(facingLeft), false});
            entitySprite = jumpUp;
        }
        if (velocity.y < 0) {
            fall.setFlip({(facingLeft), false});
            entitySprite = fall;
        }
    }
    if (blinkTime > 2.5f) {
        death.setFlip({(facingLeft), false});
        entitySprite = death;
        invincible = true;
        blinkTime -= deltaTime;
    }

    spriteComponent->setSprite(entitySprite);
    // Flips sprite for gravity room
    if (world->GetGravity().y > 0) {
        entitySprite.setFlip({false, true});
        if (facingLeft)
            entitySprite.setFlip({true, true});
        spriteComponent->setSprite(entitySprite);
    }
}
