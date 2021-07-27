#pragma once
#include "api/astructs.h"
#include "api/aenums.h"

ATexture* atexture_create(AWrap wrap, AFilter filter);
ATexture* atexture_set_data(ATexture* texture, int width, int height, int channels, void* data);
void atexture_bind(ATexture* texture, uint slot);
void atexture_delete(ATexture* texture);