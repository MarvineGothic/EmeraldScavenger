//
// Created by Sergiy Isakov on 21.11.2018.
//

#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "EmeraldGame.hpp"
#include "PhysicsComponent.hpp"
#include "CollectibleItem.hpp"

CollectibleItem::CollectibleItem(GameObject *gameObject) : Entity(gameObject) {
    emerald = EmeraldGame::gameInstance->obstaclesAtlas->get("diamond blue.png");
    pie = EmeraldGame::gameInstance->gameSpritesAtlas->get("spr_teamCakeLogo.png");
    emerald.setScale(EmeraldGame::scale / 2.0f);
    pie.setScale(EmeraldGame::scale / 3.0f);
}

void CollectibleItem::initCollectible(vec2 pos, string name) {
    this->name = name;
    this->position = pos;
    if (name == "Emerald") {
        spriteComponent->setSprite(emerald);
        physicsComponent->initCircle(b2_staticBody,
                                     emerald.getSpriteSize().x / (physicsScale * Level::tileSize),
                                     pos * Level::tileSize / physicsScale,
                                     1);
    } else if (name == "Pie") {
        spriteComponent->setSprite(pie);
        physicsComponent->initCircle(b2_staticBody,
                                     pie.getSpriteSize().x / (physicsScale * Level::tileSize),
                                     pos * Level::tileSize / physicsScale,
                                     1);
    }
    gameObject->setPosition(pos * Level::tileSize);
    physicsComponent->setSensor(true);
}

void CollectibleItem::update(float deltaTime) {
    Component::update(deltaTime);
}

vec2 CollectibleItem::getPosition() {
    return position;
}

string CollectibleItem::getName() {
    return name;
}
