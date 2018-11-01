//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include "MovingPlatformComponent.hpp"
#include "PlatformComponent.hpp"
#include "GameObject.hpp"

MovingPlatformComponent::MovingPlatformComponent(GameObject *gameObject) : Component(gameObject) {
    platformComponent = gameObject->getComponent<PlatformComponent>();
}

void MovingPlatformComponent::update(float deltaTime) {
    totalTime += deltaTime;

    // todo replace with easing function
    if (fmod(totalTime, 2) > 1) {
        platformComponent->moveTo(movementEnd);
    } else {
        platformComponent->moveTo(movementStart);
    }
    /*b2Body *body;
    platformComponent->SetAwake(true);
    if (upDown->GetLinearVelocity().y <= 0 && upDown->GetPosition().y <= moveFrom) {
        b2Vec2 velocity(0, 100);
        upDown->SetLinearVelocity(velocity);
    }
    else if (upDown->GetLinearVelocity().y >= 0 && upDown->GetPosition().y >= moveTo) {
        b2Vec2 velocity(0, -100);
        upDown->SetLinearVelocity(velocity);
    }*/
}

void MovingPlatformComponent::setMovementStart(vec2 pos) {
    movementStart = pos;
}

void MovingPlatformComponent::setMovementEnd(vec2 pos) {
    movementEnd = pos;
}
