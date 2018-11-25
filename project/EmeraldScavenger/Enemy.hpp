//
// Created by Sergiy Isakov on 05.11.2018.
//

#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "sre/Sprite.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class GameObject;

class Enemy : public Component, public b2RayCastCallback {
public:
    explicit Enemy(GameObject *gameObject);

    void setSprites(Sprite move1,
                    Sprite move2,
                    Sprite move3,
                    Sprite move4,
                    Sprite dead1,
                    Sprite dead2,
                    vec2 scale);
    
    enum class EnemyType {
        Zombie,
        Dragon,
        AngryBird,
    };

    void updateSprite(float deltaTime);

    void update(float deltaTime) override;

    bool onKey(SDL_Event &event) override;

    void jump();

    // raycast callback
    virtual float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                                  const b2Vec2 &normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;
    
    void initEnemy(std::shared_ptr<sre::SpriteAtlas> enemyAtlas, vec2 position, EnemyType enemyType);

private:
    Sprite move1;
    Sprite move2;
    Sprite move3;
    Sprite move4;
    Sprite dead1;
    Sprite dead2;
    
    static const vec2 dragonScale;
    static const vec2 birdScale;

    vector<shared_ptr<GameObject>> enemies;
    std::vector<sre::Sprite> movingSprites;
    std::vector<sre::Sprite> deadSprites;
    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> characterPhysics;
    float physicsScale;
    bool isGrounded = false;
    bool isDead = false;
    float radius;
    bool left = false;
    bool right = false;
    vec2 pos;
    float distance = 0.0f;
    bool facingLeft = true;
    int spriteIndex = 0;
    float enemyVelocity;
    bool flyingEnemy = false;
    EnemyType enemyType;
    float timePassed = 0.0f;
};
