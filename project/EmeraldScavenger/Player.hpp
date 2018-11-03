#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "sre/Sprite.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class Player : public Component, public b2RayCastCallback {
public:
    explicit Player(GameObject *gameObject);

    void setSprites(Sprite standing,
                    Sprite walk1,
                    Sprite walk2,
                    Sprite flyUp,
                    Sprite fly,
                    Sprite flyDown);

    void updateSprite(float deltaTime);

    void update(float deltaTime) override;

    bool onKey(SDL_Event &event) override;

    void jump();

    // raycast callback
    virtual float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                                  const b2Vec2 &normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

private:
    Sprite standing;
    Sprite walkLeft;
    Sprite walkRight;
    Sprite flyUp;
    Sprite fly;
    Sprite flyDown;

    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> characterPhysics;
    bool isGrounded = false;
    bool isDead = false;
    float radius;
    bool left = false;
    bool right = false;
};
