#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>
#include "glm/glm.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class GameObject;

class PlatformComponent : public Component {
public:
    explicit PlatformComponent(GameObject *gameObject);

    void initPlatform(int x,
                      int y,
                      Sprite sprite,
                      int width,
                      bool kinematic);

    void initWall(int x, int y, Sprite sprite, int height);

    void moveTo(vec2 tilePos);

    void update(float deltaTime) override;

    vec2 getPosition();

    shared_ptr<PhysicsComponent> getPhysicsComponent();

    static const vec2 tileScale;
    static const vec2 brickScale;

private:
    vector<shared_ptr<GameObject>> tiles;
    shared_ptr<PhysicsComponent> physicsComponent;
    bool kinematic;
    vec2 pos;
    int width;
    float physicsScale;
};
