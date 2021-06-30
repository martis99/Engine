#pragma once
#include "assets/texture.h"

typedef struct Sprite {
	Texture* texture;
	vec4 color;
	int sub;
	vec2i pos;
	vec2i size;
	vec4 borders;
} Sprite;

Sprite sprite_create(Texture* texture, vec4 color, vec4 borders);
Sprite sprite_create_sub(Texture* texture, vec4 color, vec2i pos, vec2i size);