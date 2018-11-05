#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "sre/Sprite.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class Player : public Component, public b2RayCastCallback {
public:
    explicit Player(GameObject *gameObject);

    void setSprites(Sprite idle,
                    Sprite jumpUp,
                    Sprite fall,
                    Sprite run1,
                    Sprite run2,
                    Sprite run3,
                    Sprite death);

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
    Sprite idle;
    Sprite jumpUp;
    Sprite fall;
    Sprite run1;
    Sprite run2;
    Sprite run3;
    Sprite death;

    vector<sre::Sprite> runningSprites;
    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> characterPhysics;

    float accelerationSpeed;
    float maximumVelocity;
    int spriteIndex = 0;
    float distance = 0.0f;
    bool isGrounded = false;
    bool isDead = false;
    float radius;
    bool left = false;
    bool right = false;
    bool facingLeft = false;
};
