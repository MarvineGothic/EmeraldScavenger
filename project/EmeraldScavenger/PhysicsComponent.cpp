//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <Box2D/Box2D.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "EmeraldGame.hpp"

PhysicsComponent::PhysicsComponent(GameObject *gameObject)
        : Component(gameObject)
{
    world = EmeraldGame::gameInstance->world;
}

PhysicsComponent::~PhysicsComponent() {
    EmeraldGame::gameInstance->deregisterPhysicsComponent(this);

    delete polygon;
    delete circle;
    if (body != nullptr && fixture!= nullptr ) {
        body->DestroyFixture(fixture);
        fixture = nullptr;
    }
    if (world != nullptr && body != nullptr ) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void PhysicsComponent::addImpulse(vec2 force) {
    b2Vec2 iForceV{force.x,force.y};
    body->ApplyLinearImpulse(iForceV, body->GetWorldCenter(), true);
}

void PhysicsComponent::addForce(vec2 force) {
    b2Vec2 forceV{force.x,force.y};
    body->ApplyForce(forceV,body->GetWorldCenter(),true);
}

vec2 PhysicsComponent::getLinearVelocity() {
    b2Vec2 v = body->GetLinearVelocity();
    return {v.x,v.y};
}

void PhysicsComponent::setLinearVelocity(vec2 velocity) {
    b2Vec2 v{velocity.x, velocity.y};
    if (velocity != vec2(0,0)){
        body->SetAwake(true);
    }
    body->SetLinearVelocity(v);
}

void PhysicsComponent::initCircle(b2BodyType type, float radius, vec2 center, float density) {
    assert(body == nullptr);
    autoUpdate = type != b2_staticBody;
    // do init
    shapeType = b2Shape::Type::e_circle;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    circle = new b2CircleShape();
    circle->m_radius = radius;
    b2FixtureDef fxD;
    fxD.userData = (void*)"Circle";
    fxD.shape = circle;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    EmeraldGame::gameInstance->registerPhysicsComponent(this);
}

void PhysicsComponent::initBox(b2BodyType type, vec2 size, vec2 center, float density) {
    assert(body == nullptr);
    autoUpdate = type != b2_staticBody;
    // do init
    shapeType = b2Shape::Type::e_polygon;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    polygon = new b2PolygonShape();
    polygon->SetAsBox(size.x, size.y, {0,0}, 0);
    b2FixtureDef fxD;
    fxD.userData = (void*)"Box";
    fxD.shape = polygon;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);



    EmeraldGame::gameInstance->registerPhysicsComponent(this);
}

bool PhysicsComponent::isSensor() {
    return fixture->IsSensor();
}

void PhysicsComponent::setSensor(bool enabled) {
    fixture->SetSensor(enabled);
}

void PhysicsComponent::fixRotation() {
    fixture->GetBody()->SetFixedRotation(true);
}

void PhysicsComponent::moveTo(vec2 pos) {
    vec2 delta = pos - getPosition();

    setLinearVelocity(delta*(1/EmeraldGame::timeStep));
}

vec2 PhysicsComponent::getPosition() {
    return vec2(body->GetPosition().x,body->GetPosition().y);
}

bool PhysicsComponent::isAutoUpdate() const {
    return autoUpdate;
}

void PhysicsComponent::setAutoUpdate(bool autoUpdate) {
    PhysicsComponent::autoUpdate = autoUpdate;
}

b2Body *PhysicsComponent::getBody() {
    return body;
}

b2Fixture *PhysicsComponent::getFixture() {
    return fixture;
}
