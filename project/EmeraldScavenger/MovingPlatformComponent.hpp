#pragma once

#include <memory>
#include "Component.hpp"
#include "glm/glm.hpp"
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>

using namespace std;
using namespace sre;
using namespace glm;

class PlatformComponent;

class MovingPlatformComponent : public Component {
public:
    explicit MovingPlatformComponent(GameObject *gameObject);

    void setMovementStart(vec2 pos);

    void setMovementEnd(vec2 pos);

    void setBody(b2Body *body);

    void update(float deltaTime) override;

private:
    float totalTime = 0;
    vec2 movementStart = vec2(0, 0);
    vec2 movementEnd = vec2(0, 0);
    b2Body *body;
    std::shared_ptr<PlatformComponent> platformComponent;
};
