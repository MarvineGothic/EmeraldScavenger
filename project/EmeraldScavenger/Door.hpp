//
// Created by Sergiy Isakov on 21.11.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_DOOR_H
#define SIMPLERENDERENGINEPROJECT_DOOR_H


#include <memory>
#include "Component.hpp"
#include "Entity.hpp"

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
	int level;

    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> physicsComponent;

    void update(float deltaTime) override;

    void initDoor(vec2 pos, bool isOpen, bool isExit, int level);

    void openDoor();

    void closeDoor();
};


#endif //SIMPLERENDERENGINEPROJECT_DOOR_H
