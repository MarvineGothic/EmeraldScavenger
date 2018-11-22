//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include "MovingPlatformComponent.hpp"
#include "PlatformComponent.hpp"
#include "GameObject.hpp"

MovingPlatformComponent::MovingPlatformComponent(GameObject *gameObject) : Component(gameObject) {
    platformComponent = gameObject->getComponent<PlatformComponent>();
}

float easeInOutQuad(float t) {
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

void MovingPlatformComponent::update(float deltaTime) {
    totalTime += deltaTime;

    float total_mod = fmod(totalTime, speed);
    float t_val = easeInOutQuad(total_mod / speed);

    if (prev_total_mod > total_mod) {
        vec2 tmp = movementStart;
        movementStart = movementEnd;
        movementEnd = tmp;
    }
    platformComponent->moveTo(glm::mix(movementStart, movementEnd, t_val));
    prev_total_mod = total_mod;
}

void MovingPlatformComponent::setMovementStart(vec2 pos) {
    movementStart = pos;
}

void MovingPlatformComponent::setMovementEnd(vec2 pos) {
    movementEnd = pos;
}

void MovingPlatformComponent::setBody(b2Body *body) {
    this->body = body;
}

void MovingPlatformComponent::setSpeed(float speed) {
    this->speed = speed;
}