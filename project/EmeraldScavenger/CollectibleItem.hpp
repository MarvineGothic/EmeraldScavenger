//
// Created by Sergiy Isakov on 21.11.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_COLLECTIBLEITEM_H
#define SIMPLERENDERENGINEPROJECT_COLLECTIBLEITEM_H

#include <memory>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "Entity.hpp"


class CollectibleItem : public Component {
    float physicsScale;
    vec2 position;
    string name;

    Sprite emerald;
    Sprite pie;
public:
    explicit CollectibleItem(GameObject *gameObject);
    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> physicsComponent;

    vec2 getPosition();

    string getName();

    void initCollectible(vec2 pos, string name);

    void update(float deltaTime) override;

};


#endif //SIMPLERENDERENGINEPROJECT_COLLECTIBLEITEM_H
