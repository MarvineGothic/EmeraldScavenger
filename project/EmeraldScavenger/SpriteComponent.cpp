//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include <iostream>
#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include <cmath>
#include <SDL_quit.h>


SpriteComponent::SpriteComponent(GameObject *gameObject) : Component(gameObject) {}

void SpriteComponent::renderSprite(SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {
    sprite.setPosition(gameObject->getPosition());
    sprite.setRotation(static_cast<float>(gameObject->getRotation() * (180 / M_PI)));
    spriteBatchBuilder.addSprite(sprite);
}

void SpriteComponent::setSprite(const Sprite &sprite) {
    this->sprite = sprite;
}

Sprite SpriteComponent::getSprite() {
    return sprite;
}
