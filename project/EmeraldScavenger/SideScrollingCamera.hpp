#pragma once

#include "sre/Camera.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class SideScrollingCamera : public Component {
    Camera camera;
    shared_ptr<GameObject> followObject;
    vec2 offset;
    bool zoom = false;
public:
    explicit SideScrollingCamera(GameObject *gameObject);

    void update(float deltaTime) override;

    void setFollowObject(shared_ptr<GameObject> followObject, vec2 offset);

    void setZoomMode(bool zoomEnabled);
    bool isZoomMode();

    Camera& getCamera();
    
};

