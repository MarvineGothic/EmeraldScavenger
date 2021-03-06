#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "sre/Sprite.hpp"
#include "Entity.hpp"

class Player : public Entity, public b2RayCastCallback {

    Sprite idle;
    Sprite jumpUp;
    Sprite fall;
    Sprite death;
    Sprite entitySprite;
    vector<Sprite> runningSprites;


    float accelerationSpeed;
    float maximumVelocity;
    int spriteIndex = 0;
    float distance = 0.0f;
    bool touchesPlatform = false;
    bool isGrounded = false;
    shared_ptr<GameObject> cannonObject;
    float radius;
    bool left = false;
    bool right = false;
    bool isJump = false;
    bool facingLeft = false;
    bool invincible = false;
    bool spaceKey = false;
    float blinkFreq = 0.1f;
    float blinkDelta = 0.0f;
    float blinkTime = 0.0f;
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
    
    void fireCannon(std::shared_ptr<sre::SpriteAtlas> spritesAtlas);

    // raycast callback
    virtual float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                                  const b2Vec2 &normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    bool inPit = false;
    bool lostLife = false;
    bool exit = false;
    float fireTimer = 0.0f;

};
