//
// Created by Sergiy Isakov on 21.11.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_COLLECTIBLEITEM_H
#define SIMPLERENDERENGINEPROJECT_COLLECTIBLEITEM_H

#include <memory>
#include "Component.hpp"
#include "SpriteComponent.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class CollectibleItem : public Component {
    float physicsScale;
    vec2 position;
    string name;

    Sprite emerald;
    Sprite pie;
public:
    explicit CollectibleItem(GameObject *gameObject);

    shared_ptr<GameObject> gameObjectCopy;
    shared_ptr<SpriteComponent> collectibleSpriteComponent;
    shared_ptr<PhysicsComponent> collectiblePhysicsComponent;

    vec2 getPosition();

    string getName();

    void initCollectible(vec2 pos, string name);

    void update(float deltaTime) override;

};


#endif //SIMPLERENDERENGINEPROJECT_COLLECTIBLEITEM_H
