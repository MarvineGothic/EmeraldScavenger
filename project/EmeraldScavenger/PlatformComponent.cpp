//
// Created by Morten Nobel-Jørgensen on 11/4/17.
//

#include <Box2D/Box2D.h>
#include <iostream>
#include <glm/ext.hpp>
#include "PlatformComponent.hpp"
#include "EmeraldGame.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

const vec2 PlatformComponent::tileScale(0.85f, 0.85f);
const vec2 PlatformComponent::brickScale(0.3f, 0.3f);

PlatformComponent::PlatformComponent(GameObject *gameObject) : Component(gameObject) {
    auto game = EmeraldGame::gameInstance;
    physicsScale = game->physicsScale;
}

void PlatformComponent::initPlatform(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId,
                                     int width,
                                     bool kinematic) {
    this->kinematic = kinematic;
    this->width = width;
    auto game = EmeraldGame::gameInstance;
    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas->get("tile ground.png");
    sprite.setScale(tileScale);
    float tileSize = Level::tileSize;
    vec2 offset{tileSize / 2, tileSize / 2};
    vec2 pos = offset + vec2{x * tileSize, y * tileSize};

    gameObject->setPosition(pos);
    this->pos = pos;

    spriteComponent->setSprite(sprite);

    physicsComponent = gameObject->addComponent<PhysicsComponent>();

    pos.x += (width - 1) * 0.5f * tileSize;
    physicsComponent->initBox(kinematic ? b2_kinematicBody : b2_staticBody,
                              vec2{tileSize * width, tileSize} / physicsScale * 0.5f, pos / physicsScale, 0);

    physicsComponent->setAutoUpdate(false);
    physicsComponent->getFixture()->SetFriction(0.5f);
    for (int i = 1; i < width; i++) {
        auto go = game->createGameObject();
        spriteComponent = go->addComponent<SpriteComponent>();
        int spriteOffset = i == width - 1 ? 2 : 1; // select middle or right piece
        //vec2 offset{tileSize / 2, tileSize / 2};
        pos = offset + vec2{(x + i) * tileSize, y * tileSize};
        go->setPosition(pos);
        spriteComponent->setSprite(sprite);
        tiles.push_back(go);
    }
}

void PlatformComponent::initWall(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId,
                                 int height) {
    this->kinematic = false; // walls cannot be moved
    auto game = EmeraldGame::gameInstance;

    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas->get("brick_1.png");
    sprite.setScale(brickScale);
    float tileSize = Level::tileSize;
    vec2 offset{tileSize / 2, tileSize / 2};
    pos = offset + vec2{x * tileSize, y * tileSize};

    gameObject->setPosition(pos);

    spriteComponent->setSprite(sprite);
    physicsComponent = gameObject->addComponent<PhysicsComponent>();
    pos.y += (height - 1) * 0.5f * tileSize;
    physicsComponent->initBox(kinematic ? b2_kinematicBody : b2_staticBody,
                              vec2{tileSize, tileSize * height} / physicsScale * 0.5f, pos / physicsScale, 0);
    physicsComponent->setAutoUpdate(false);

    for (int i = 1; i < height; i++) {
        auto go = game->createGameObject();
        spriteComponent = go->addComponent<SpriteComponent>();
        int spriteOffset = i == height - 1 ? 0 : 30; // select middle or right piece
        sprite = spriteAtlas->get("brick_1.png");
        sprite.setScale(brickScale);
        vec2 offset{tileSize / 2, tileSize / 2};
        vec2 pos = offset + vec2{x * tileSize, (y + i) * tileSize};
        go->setPosition(pos);
        spriteComponent->setSprite(sprite);

        tiles.push_back(go);
    }
}


void PlatformComponent::moveTo(vec2 tilePos) {
    assert(kinematic);
    vec2 offset{Level::tileSize / 2, Level::tileSize / 2};
    this->pos = offset + vec2{tilePos.x * Level::tileSize, tilePos.y * Level::tileSize};

    gameObject->setPosition(pos);

    vec2 physPos = pos;
    physPos.x += (width - 1) * 0.5f * Level::tileSize;
    physicsComponent->moveTo(physPos / physicsScale);

    for (auto &tile : tiles) {
        pos.x += Level::tileSize;
        tile->setPosition(pos);
    }
}

vec2 PlatformComponent::getPosition() {
    return pos;
}

void PlatformComponent::update(float deltaTime) {
    if (kinematic) {
        vec2 physPos = pos;
        physPos.x += (width - 1) * 0.5f * Level::tileSize;
        physicsComponent->moveTo(physPos / physicsScale);
    }
}

shared_ptr<PhysicsComponent> PlatformComponent::getPhysicsComponent() {
    return physicsComponent;
}
