#pragma once
#include "api/atypes.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data);
void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot);
void atexture_delete(ATexture* texture);