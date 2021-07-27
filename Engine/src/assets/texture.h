#pragma once
#include "structs.h"

Texture* texture_create(Texture* texture, AWrap wrap, AFilter filter);
Texture* texture_create_from_image(Texture* texture, Image* image, AWrap wrap, AFilter filter);
Texture* texture_set_image(Texture* texture, Image* image);
void texture_bind(Texture* texture, int slot);
void texture_delete(Texture* texture);