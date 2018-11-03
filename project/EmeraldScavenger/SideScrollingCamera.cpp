#include <utility>

//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "SideScrollingCamera.hpp"
#include "EmeraldGame.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static const std::string platform="Windows";
#else
static const std::string platform="Mac";
#endif

SideScrollingCamera::SideScrollingCamera(GameObject *gameObject)
        : Component(gameObject) {
    setZoomMode(false);
}

Camera &SideScrollingCamera::getCamera() {
    return camera;
}

void SideScrollingCamera::update(float deltaTime) {
    if (followObject != nullptr) {
        auto objectPos = followObject->getPosition();
        auto position = objectPos;
        auto windowWidth = EmeraldGame::windowSize.x;
        auto windowHeight = EmeraldGame::windowSize.y;
        auto levelWidth = EmeraldGame::gameInstance->getLevel()->getWidth();
        auto levelHeight = EmeraldGame::gameInstance->getLevel()->getHeight();

        // sets game camera to behave correctly on Mac
        auto macWidthOffset = 0;
        auto macHeightOffset = 0;
        if (platform=="Mac") {
            std::cout << "You are on Mac" << std::endl;
            windowWidth = windowWidth / 2;
            windowHeight = windowHeight / 2;
            macWidthOffset += windowWidth / 2;
            macHeightOffset += windowHeight / 2;
        }

        position.x = offset.x;
        position.y = offset.y;
        // start moving camera when player walks out of half of window size:
        if (objectPos.x > windowWidth / 2 && levelWidth > windowWidth)
            position.x = objectPos.x + macWidthOffset;
        if (objectPos.y > windowHeight / 2 && levelHeight > windowHeight)
            position.y = objectPos.y  + macHeightOffset;
        // stop moving camera when on the edge of the world:
        if ((objectPos.x + (windowWidth / 2)) > levelWidth && levelWidth > windowWidth) {
            position.x = levelWidth;
        }
        if ((objectPos.y + (windowHeight / 2)) > levelHeight && levelHeight > windowHeight) {
            position.y = levelHeight;
        }

        if (zoom) {
            position.x -= offset.x;
            position.y = offset.y / 2;
        }
        gameObject->setPosition(position);
        vec3 eye(position, 0);
        vec3 at(position, -1);
        vec3 up(0, 1, 0);
        camera.lookAt(eye, at, up);
    }
}

void SideScrollingCamera::setFollowObject(shared_ptr<GameObject> followObject, vec2 offset) {
    this->followObject = move(followObject);
    this->offset = offset;
}

void SideScrollingCamera::setZoomMode(bool zoomEnabled) {
    this->zoom = zoomEnabled;
    if (zoomEnabled) {
        camera.setOrthographicProjection(EmeraldGame::windowSize.y / 4, -1, 1);
    } else {
        camera.setOrthographicProjection(EmeraldGame::windowSize.y / 2, -1, 1);
    }
}

bool SideScrollingCamera::isZoomMode() {
    return zoom;
}
