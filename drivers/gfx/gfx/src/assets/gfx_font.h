#pragma once
#include "gfx_types.h"

EFont *font_load(EFont *font, Renderer *renderer, const char *path, int size);
FontCharacter font_get_char(EFont *font, char c);
void font_delete(EFont *font, Renderer *renderer);
