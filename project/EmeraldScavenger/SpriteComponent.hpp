#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"

using namespace sre;

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(GameObject *gameObject);

    void renderSprite(SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) override;

    void setSprite(const Sprite &sprite);

    Sprite getSprite();

private:
    Sprite sprite;
};

