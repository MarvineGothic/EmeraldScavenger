//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include "MovingPlatformComponent.hpp"
#include "PlatformComponent.hpp"
#include "GameObject.hpp"

MovingPlatformComponent::MovingPlatformComponent(GameObject *gameObject) : Component(gameObject) {
    platformComponent = gameObject->getComponent<PlatformComponent>();
}

void MovingPlatformComponent::update(float deltaTime) {
    totalTime += deltaTime;

    movementStart.y += 0.01f;

    platformComponent->moveTo(movementStart);
    // todo replace with easing function
    /*if (fmod(totalTime, 2) > 2) {
        platformComponent->moveTo(movementEnd);
    } else {
        platformComponent->moveTo(movementStart);
    }*/
    /*body->SetAwake(true);
    if (body->GetLinearVelocity().y <= 0 && body->GetPosition().y <= movementStart.y) {
        b2Vec2 velocity(0, 100);
        body->SetLinearVelocity(velocity);
    } else if (body->GetLinearVelocity().y >= 0 && body->GetPosition().y >= movementEnd.y) {
        b2Vec2 velocity(0, -100);
        body->SetLinearVelocity(velocity);
    }*/
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
