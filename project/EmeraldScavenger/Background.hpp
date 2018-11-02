#pragma once

#include "Component.hpp"
#include "sre/SpriteBatch.hpp"
#include "sre/Texture.hpp"
#include "sre/RenderPass.hpp"

using namespace sre;
using namespace std;

class Background {
public:
	Background();
	void init(string filename);
	void resetBackground();
	void renderBackground(RenderPass& renderPass, float offset);
private:
	shared_ptr<Texture> tex;
	shared_ptr<SpriteBatch> batch;
};
