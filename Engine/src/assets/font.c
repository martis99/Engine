#include "pch.h"

#include "assets.h"

static Font* load_data(Font* font, const char* path) {
	FILE* file;
	fopen_s(&file, path, "rb");

	if (file == NULL) {
		log_error("Failed to open font file");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	font->data_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	font->data = m_malloc(font->data_size);

	if (font->data == 0) {
		log_error("Failed to allocate memory");
		return NULL;
	}

	fread(font->data, font->data_size, 1, file);
	fclose(file);

	if (!stbtt_InitFont(&font->info, font->data, 0)) {
		log_error("Failed to init TrueType\n");
		return NULL;
	}

	return font;
}

static void load_characters(Font* font, float scale, int* area) {
	*area = 0;

	int i = 0;
	for (uint c = FIRST_CHARACTER; c < LAST_CHARACTER; c++) {
		int advance, lsb;
		stbtt_GetCodepointHMetrics(&font->info, c, &advance, &lsb);
		int x0, y0, x1, y1;
		stbtt_GetCodepointBitmapBox(&font->info, c, scale, scale, &x0, &y0, &x1, &y1);
		int width = x1 - x0;
		int height = y1 - y0;

		*area += (width * height);

		font->characters[i].c = c;
		font->characters[i].size = (vec2i){ width, height };
		font->characters[i].advance = (int)(advance * scale);
		font->characters[i].offset = (vec2i){ x0, y0 };
		i++;
	}
}

static void sort_characters(Font* font, int* map) {
	int n = 0;
	bool visited[CHARACTERS_COUNT];
	for (int i = 0; i < CHARACTERS_COUNT; i++) {
		visited[i] = 0;
	}
	while (1) {
		int max = -1;
		for (int i = 0; i < CHARACTERS_COUNT; i++) {
			if (visited[i] == 0 && font->characters[i].size.y > max) {
				max = font->characters[i].size.y;
			}
		}
		if (max == -1) {
			break;
		}
		for (int i = 0; i < CHARACTERS_COUNT; i++) {
			if (font->characters[i].size.y == max) {
				visited[i] = 1;
				map[n] = i;
				n++;
			}
		}
	}
}

static Texture* create_texture(Font* font, Assets* assets, float scale, int area) {
	int map[CHARACTERS_COUNT];
	sort_characters(font, map);

	uint s = (uint)(sqrt(area) * 1.2);
	uint x = 0;
	uint y = 0;
	uint w = s;
	uint h = s;

	uint append = font->characters[map[0]].size.y;
	Image img;
	image_create(&img, w, h, 1);
	for (int i = 0; i < CHARACTERS_COUNT; i++) {
		FontCharacter* fc = &font->characters[map[i]];

		if (x + fc->size.x > w) {
			x = 0;
			y += append;
			append = fc->size.y;
		}

		fc->pos = (vec2i){ x, y };
		stbtt_MakeCodepointBitmap(&font->info, img.data + x + (size_t)y * w, fc->size.x, fc->size.y, w, scale, scale, fc->c);

		x += fc->size.x;
	}

	Texture* texture = assets_texture_create_from_image(assets, "font", &img, F_NEAREST);
	image_delete(&img);
	return texture;
}

Font* font_load(Font* font, Assets* assets, const char* path, int size) {
	if (load_data(font, path) == NULL) {
		log_error("Failed to load font");
		return NULL;
	}

	float scale = stbtt_ScaleForPixelHeight(&font->info, (float)size);

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&font->info, &ascent, &descent, &lineGap);

	font->ascent = (int)(ascent * scale);
	font->descent = (int)(descent * scale);
	font->line_height = (int)((abs(ascent) + abs(descent)) * scale);

	int area;
	load_characters(font, scale, &area);
	font->texture = create_texture(font, (Assets*)assets, scale, area);
	return font;
}

FontCharacter font_get_char(Font* font, char c) {
	return font->characters[c - FIRST_CHARACTER];
}

void font_delete(Font* font) {
	m_free(font->data, font->data_size);
}