#pragma once

#include "api/gfx/atexture.h"
#include "image.h"

typedef struct Texture {
	ATexture* texture;
	int width;
	int height;
	int channels;
} Texture;

Texture* texture_create(Texture* texture, AFilter filter);
Texture* texture_create_from_image(Texture* texture, Image* image, AFilter filter);
Texture* texture_set_image(Texture* texture, Image* image);
void texture_bind(Texture* texture, int slot);
void texture_delete(Texture* texture);