#pragma once

#include "./sdk/math/color_t.hpp"
#include "./sdk/math/math.hpp"

namespace renderer
{
	void rect(float x, float y, float w, float h, color_t color);
	void filled_rect(float x, float y, float w, float h, color_t color);
	void text(float x, float y, color_t color, const char* text);
	void text_centered(float x, float y, float w, float h, color_t color, const char* text);
	void line(float x, float y, float x1, float y1, color_t color);
}