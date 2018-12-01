//
// Created by Admin on 01.12.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_ENTITY_H
#define SIMPLERENDERENGINEPROJECT_ENTITY_H

#include <memory>
#include "Component.hpp"
#include "SpriteComponent.hpp"

using namespace std;
using namespace sre;
using namespace glm;

enum class EntityType {
    PLAYER,
    ENEMY,
    BRICK,
    ROCK
};

class Entity : public Component {

public:
    explicit Entity(GameObject *gameObject);
    float physicsScale;
    Sprite entitySprite;

    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> physicsComponent;

};


#endif //SIMPLERENDERENGINEPROJECT_ENTITY_H
