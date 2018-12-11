//
// Created by Sergiy Isakov on 01.12.2018.
//

#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "EmeraldGame.hpp"
#include "PhysicsComponent.hpp"
#include "Entity.hpp"
#include "Level.hpp"

Entity::Entity(GameObject *gameObject) : Component(gameObject) {
    spriteComponent = gameObject->addComponent<SpriteComponent>();
    physicsComponent = gameObject->addComponent<PhysicsComponent>();
    physicsScale = EmeraldGame::gameInstance->physicsScale;
}