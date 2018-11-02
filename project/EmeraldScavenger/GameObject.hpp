#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "sre/SpriteBatch.hpp"

#pragma once

using namespace std;
using namespace sre;
using namespace glm;

// Forward declaration
class Component;

// GameObject are empty container objects, which contains Components
class GameObject {
    vector<shared_ptr<Component>> components;

    vec2 position;
    float rotation;
public:

    GameObject() = default;

    ~GameObject();

    template<class T>
    // Add component of a given type to a gameObject. example:
    // shared_ptr<SpriteComponent> spriteComponent = gameObject->addComponent<SpriteComponent>();
    shared_ptr<T> addComponent();

    template<class T>
    //  Get component of a given type to a gameObject. If not found return empty shared_ptr (==nullptr). example:
    // shared_ptr<SpriteComponent> spriteComponent = gameObject->getComponent<SpriteComponent>();
    shared_ptr<T> getComponent();

    bool removeComponent(shared_ptr<Component> component);

    void renderSprite(SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder);

    void update(float deltaTime);

    const vec2 &getPosition() const;

    void setPosition(const vec2 &position);

    float getRotation() const;

    void setRotation(float rotation);

    const vector<shared_ptr<Component>> &getComponents();

    string name = "_";

    friend class EmeraldGame;
};

// definition of the template member function addComponent
// Usage:
// GameObject* go = something;
// shared_ptr<SpriteComponent> sc = go->addComponent<SpriteComponent>();
template<class T>
inline shared_ptr<T> GameObject::addComponent() {
    auto obj = shared_ptr<T>(new T(this));
    components.push_back(obj);

    return obj;
}

// definition of the template member function addComponent
// Usage:
// GameObject* go = something;
// shared_ptr<SpriteComponent> sc = go->addComponent<SpriteComponent>();
template<class T>
inline shared_ptr<T> GameObject::getComponent() {
    for (auto c : components) {
        shared_ptr<T> res = dynamic_pointer_cast<T>(c);
        if (res != nullptr) {
            return res;
        }
    }
    return shared_ptr<T>();
}
