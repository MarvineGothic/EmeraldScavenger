//
// Created by Sergiy Isakov on 21.11.2018.
//

#include <SDL_events.h>
#include <iostream>
#include "Door.hpp"
#include "GameObject.hpp"
#include "EmeraldGame.hpp"
#include "PhysicsComponent.hpp"

Door::Door(GameObject *gameObject) : Component(gameObject) {
    doorSpriteComponent = gameObject->addComponent<SpriteComponent>();
    doorPhysicsComponent = gameObject->addComponent<PhysicsComponent>();
    open = EmeraldGame::gameInstance->gameSpritesAtlas->get("spr_doorOpen.png");
    close = EmeraldGame::gameInstance->gameSpritesAtlas->get("spr_door.png");
    open.setScale(EmeraldGame::scale * 1.5f);
    close.setScale(EmeraldGame::scale * 1.5f);

    physicsScale = EmeraldGame::gameInstance->physicsScale;
}

void Door::openDoor() {
    doorSpriteComponent->setSprite(open);
}

void Door::closeDoor() {
    doorSpriteComponent->setSprite(close);
}

void Door::update(float deltaTime) {
    if (EmeraldGame::gameInstance->emeraldCounter == 5 && isExit)
        doorSpriteComponent->setSprite(open);
}

void Door::initDoor(vec2 pos, bool isOpen, bool isExit) {
    this->isExit = isExit;
    if (isOpen)doorSpriteComponent->setSprite(open);
    else doorSpriteComponent->setSprite(close);

    doorPhysicsComponent->initBox(b2_staticBody,
                                  vec2(close.getSpriteSize()) /
                                  (physicsScale * Level::tileSize * (EmeraldGame::scale * 1.5f)),
                                  pos * Level::tileSize / physicsScale,
                                  1);
    doorPhysicsComponent->setSensor(true);
    gameObject->setPosition(vec2{pos.x, pos.y} * Level::tileSize);
}
