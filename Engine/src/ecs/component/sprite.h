#pragma once
#include "structs.h"

Sprite sprite_create(Texture* texture, vec4 color, vec4 borders);
Sprite sprite_create_sub(Texture* texture, vec4 color, vec2i pos, vec2i size);