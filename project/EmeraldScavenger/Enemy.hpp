//
// Created by Sergiy Isakov on 05.11.2018.
//

#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "sre/Sprite.hpp"
#include "Entity.hpp"

class GameObject;

class Enemy : public Entity, public b2RayCastCallback {
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
		Boulder,
        SpikeMonster,
    };

    void updateSprite(float deltaTime);

    void update(float deltaTime) override;

    void jump();

    // raycast callback
    virtual float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                                  const b2Vec2 &normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;
    
    void initEnemy(shared_ptr<SpriteAtlas> enemyAtlas, vec2 position, EnemyType enemyType);
    
    bool isDead = false;

private:
    Sprite move1;

    static const vec2 dragonScale;
    static const vec2 birdScale;
	static const vec2 spikeScale;

    vector<shared_ptr<GameObject>> enemies;
    vector<Sprite> movingSprites;
    vector<Sprite> deadSprites;

    bool isGrounded = false;
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
	float rotation = 0.0f;
};
