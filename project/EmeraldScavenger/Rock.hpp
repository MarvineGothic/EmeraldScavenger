//
// Created by Sergiy Isakov on 01.12.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_ROCK_H
#define SIMPLERENDERENGINEPROJECT_ROCK_H

#include <memory>
#include "Component.hpp"
#include "Entity.hpp"

class Rock : public Entity {

public:
    explicit Rock(GameObject *gameObject);

    void update(float deltaTime) override;

    void initRock(vec2 pos,
                  Sprite sprite,
                  vec2 spriteScale,
                  EntityType entityType,
                  float restitution,
                  float friction,
                  float density);
};

#endif //SIMPLERENDERENGINEPROJECT_ROCK_H
