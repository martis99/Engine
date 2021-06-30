#pragma once
#include "afilter.h"

typedef struct ATexture ATexture;

ATexture* atexture_create(AFilter filter);
ATexture* atexture_set_data(ATexture* texture, int width, int height, int channels, void* data);
void atexture_bind(ATexture* texture, uint slot);
void atexture_delete(ATexture* texture);