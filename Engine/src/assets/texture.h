#pragma once
#include "structs.h"

Texture* texture_create(Texture* texture, AWrap wrap, AFilter filter);
Texture* texture_create_from_image(Texture* texture, Renderer* renderer, Image* image, AWrap wrap, AFilter filter);
Texture* texture_set_image(Texture* texture, Renderer* renderer, Image* image);
void texture_bind(Texture* texture, Renderer* renderer, int slot);
void texture_delete(Texture* texture);