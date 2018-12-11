//
// Created by Sergiy Isakov on 21.11.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_DOOR_H
#define SIMPLERENDERENGINEPROJECT_DOOR_H


#include <memory>
#include "Component.hpp"
#include "Entity.hpp"

class Door : public Entity {

    Sprite open;
    Sprite close;

public:
    explicit Door(GameObject *gameObject);

    bool isExit = false;

    int level;

    vec2 nextLevelStartPosition;

    void update(float deltaTime) override;

    void initDoor(vec2 pos, bool isOpen, bool isExit, int level, vec2 nextLevelStartPosition);

    void openDoor();

    void closeDoor();
};


#endif //SIMPLERENDERENGINEPROJECT_DOOR_H
