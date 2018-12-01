//
// Created by Sergiy Isakov on 01.12.2018.
//

#include <SDL_events.h>
#include <iostream>
#include "Rock.hpp"
#include "GameObject.hpp"
#include "EmeraldGame.hpp"
#include "PhysicsComponent.hpp"

Rock::Rock(GameObject *gameObject) : Entity(gameObject) {
}

void Rock::update(float deltaTime) {
}

void Rock::initRock(vec2 pos,
                    Sprite sprite,
                    vec2 spriteScale,
                    EntityType entityType,
                    float restitution,
                    float friction,
                    float density) {

    entitySprite = sprite;
    entitySprite.setScale(spriteScale);
    spriteComponent->setSprite(entitySprite);
    if (entityType == EntityType::BRICK)
        physicsComponent->initBox(b2_dynamicBody,
                                  (vec2(entitySprite.getSpriteSize()) * spriteScale)/
                                  (physicsScale * 2.f ),
                                  pos * Level::tileSize / physicsScale,
                                  1);
    else {
        physicsComponent->initCircle(b2_dynamicBody,
                                     (vec2(entitySprite.getSpriteSize()).x * spriteScale.x)/
                                     (physicsScale * 2.f),
                                     pos * Level::tileSize / physicsScale,
                                     1
        );
    }

    physicsComponent->getFixture()->SetRestitution(restitution);
    physicsComponent->getFixture()->SetFriction(friction);
    physicsComponent->getFixture()->SetDensity(density);
    gameObject->setPosition(vec2{pos.x, pos.y} * Level::tileSize);
}