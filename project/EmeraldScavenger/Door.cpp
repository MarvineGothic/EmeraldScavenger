//
// Created by Sergiy Isakov on 21.11.2018.
//

#include <SDL_events.h>
#include <iostream>
#include "Door.hpp"
#include "GameObject.hpp"
#include "EmeraldGame.hpp"
#include "PhysicsComponent.hpp"
#include "Level.hpp"

Door::Door(GameObject *gameObject) : Component(gameObject) {
    spriteComponent = gameObject->addComponent<SpriteComponent>();
    physicsComponent = gameObject->addComponent<PhysicsComponent>();
    open = EmeraldGame::gameInstance->getGameSpriteAtlas()->get("spr_doorOpen.png");
    close = EmeraldGame::gameInstance->getGameSpriteAtlas()->get("spr_door.png");
    open.setScale(EmeraldGame::scale * 1.5f);
    close.setScale(EmeraldGame::scale * 1.5f);

    physicsScale = EmeraldGame::gameInstance->physicsScale;
}

void Door::openDoor() {
    spriteComponent->setSprite(open);
}

void Door::closeDoor() {
    spriteComponent->setSprite(close);
}

void Door::update(float deltaTime) {
    if (EmeraldGame::gameInstance->getEmeraldCounter() >= Level::getEmeraldsNeeded() && isExit && !Level::doorIsOpen) {
        spriteComponent->setSprite(open);
        EmeraldGame::gameInstance->audioManager->playSFX("doorOpen.flac", 8);
        Level::doorIsOpen = true;
    }
}

void Door::initDoor(vec2 pos, bool isOpen, bool isExit, int level, vec2 nextLevelStartPosition) {
    this->isExit = isExit;
    if (isOpen) {
        spriteComponent->setSprite(open);
        Level::doorIsOpen = true;
    } else spriteComponent->setSprite(close);

    physicsComponent->initBox(b2_staticBody,
                              vec2(close.getSpriteSize()) /
                              (physicsScale * Level::tileSize * (EmeraldGame::scale * 1.5f)),
                              pos * Level::tileSize / physicsScale,
                              1);
    physicsComponent->setSensor(true);

	this->level = level;
	this->nextLevelStartPosition = nextLevelStartPosition;
=======
    //this->level = level;

    gameObject->setPosition(vec2{pos.x, pos.y} * Level::tileSize);
}
