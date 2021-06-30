#pragma once
#include "assets/texture.h"
#include "stb/stb_truetype.h"

#define FIRST_CHARACTER 32
#define LAST_CHARACTER 127
#define CHARACTERS_COUNT LAST_CHARACTER - FIRST_CHARACTER

typedef struct Assets Assets;

typedef struct FontCharacter {
	unsigned char c;
	vec2i pos;
	vec2i size;
	int advance;
	vec2i offset;
} FontCharacter;

typedef struct Font {
	unsigned char* data;
	size_t data_size;
	stbtt_fontinfo info;
	FontCharacter characters[CHARACTERS_COUNT];
	Texture* texture;
	int ascent;
	int descent;
	int line_height;
} Font;

Font* font_load(Font* font, Assets* assets, const char* path, int size);
FontCharacter font_get_char(Font* font, char c);
void font_delete(Font* font);