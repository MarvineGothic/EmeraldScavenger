#include <utility>

//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include <sre/SpriteAtlas.hpp>
#include <sre/Texture.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>
#include "Background.hpp"
#include "EmeraldGame.hpp"

Background::Background() {
}

void Background::renderBackground(RenderPass &renderPass, float offset) {
    renderPass.draw(batch, translate(vec3(offset, 0, 0)));
}

void Background::init(string filename) {
    tex = Texture::create().withFile(move(filename))
            .withFilterSampling(false)
            .build();

    auto backgroundAtlas = SpriteAtlas::createSingleSprite(tex, "background", vec2(0, 0));
    auto backgroundSprite = backgroundAtlas->get("background");
    float scaleY = EmeraldGame::windowSize.y / tex->getHeight();
    float scaleX = EmeraldGame::windowSize.x / tex->getWidth();
    backgroundSprite.setScale({scaleY, scaleY});
    auto batchBuilder = SpriteBatch::create();
    for (int i = 0; i < 100; i++) {
        backgroundSprite.setPosition(vec2(tex->getWidth() * (i - 1) * scaleY, 0));
        batchBuilder.addSprite(backgroundSprite);
    }
    batch = batchBuilder.build();
}
void Background::resetBackground(){
    batch.reset();
}

