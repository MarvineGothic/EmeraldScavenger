//
// Created by Sergiy Isakov on 21.11.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_DOOR_H
#define SIMPLERENDERENGINEPROJECT_DOOR_H


#include <memory>
#include "Component.hpp"
#include "SpriteComponent.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class Door : public Component {
    float physicsScale;

    Sprite open;
    Sprite close;

public:
    explicit Door(GameObject *gameObject);

    bool isExit = false;

    shared_ptr<SpriteComponent> doorSpriteComponent;
    shared_ptr<PhysicsComponent> doorPhysicsComponent;

    void update(float deltaTime) override;

    void initDoor(vec2 pos, bool isOpen, bool isExit);

    void openDoor();

    void closeDoor();
};


#endif //SIMPLERENDERENGINEPROJECT_DOOR_H
