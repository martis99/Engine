#pragma once
#include "gfx/gfx_types.h"

Texture *texture_create(Texture *texture, Renderer *renderer, Image *image, AWrap wrap, AFilter filter);
void texture_bind(Texture *texture, Renderer *renderer, int slot);
void texture_delete(Texture *texture, Renderer *renderer);
