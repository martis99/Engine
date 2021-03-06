#pragma once
#include "gfx_types.h"

Font* font_load(Font* font, Renderer* renderer, const char* path, int size);
FontCharacter font_get_char(Font* font, char c);
void font_delete(Font* font, Renderer* renderer);