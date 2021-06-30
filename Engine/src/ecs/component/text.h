#pragma once
#include "assets/font.h"

typedef struct Text {
	const char* text;
	Font* font;
	vec4 color;
} Text;

Text text_create(const char* text, Font* font, vec4 color);