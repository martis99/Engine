#include "pch.h"
#include "sprite.h"

Sprite sprite_create(Texture* texture, vec4 color, vec4 borders) {
	return (Sprite) { texture, color, 0, (vec2i) { 0, 0 }, (vec2i) { 0, 0 }, borders };
}

Sprite sprite_create_sub(Texture* texture, vec4 color, vec2i pos, vec2i size) {
	return (Sprite) {
		texture, color, 1, pos, size, (vec4) { 0, 0, 0, 0 }
	};
}