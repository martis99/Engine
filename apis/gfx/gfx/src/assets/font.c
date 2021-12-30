#include "assets/image.h"
#include "texture.h"

#include "stb/stb_truetype.h"

static Font* load_data(Renderer* renderer, Font* font, const char* path, stbtt_fontinfo* info) {
	FILE* file;
	fopen_s(&file, path, "rb");

	if (file == NULL) {
		renderer->callbacks.on_error("Failed to open font file", NULL);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	font->data_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	font->data = m_malloc(font->data_size);

	if (font->data == 0) {
		renderer->callbacks.on_error("Failed to allocate memory", NULL);
		return NULL;
	}

	fread(font->data, font->data_size, 1, file);
	fclose(file);

	if (!stbtt_InitFont(info, font->data, 0)) {
		renderer->callbacks.on_error("Failed to init TrueType\n", NULL);
		return NULL;
	}

	return font;
}

static void load_characters(Font* font, float scale, int* area, stbtt_fontinfo* info) {
	*area = 0;

	int i = 0;
	for (uint c = FIRST_CHARACTER; c < LAST_CHARACTER; c++) {
		int advance, lsb;
		stbtt_GetCodepointHMetrics(info, c, &advance, &lsb);
		int x0, y0, x1, y1;
		stbtt_GetCodepointBitmapBox(info, c, scale, scale, &x0, &y0, &x1, &y1);
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

static Texture* create_texture(Font* font, Renderer* renderer, float scale, int area, stbtt_fontinfo* info) {
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
		stbtt_MakeCodepointBitmap(info, img.data + x + (size_t)y * w, fc->size.x, fc->size.y, w, scale, scale, fc->c);

		x += fc->size.x;
	}

	font->texture = m_malloc(sizeof(Texture));
	Texture* texture = texture_create(font->texture, renderer, &img, A_CLAMP_TO_EDGE, A_NEAREST);
	image_delete(&img);
	return texture;
}

Font* font_load(Font* font, Renderer* renderer, const char* path, int size) {
	stbtt_fontinfo info;
	if (load_data(renderer, font, path, &info) == NULL) {
		renderer->callbacks.on_error("Failed to load font", NULL);
		return NULL;
	}

	float scale = stbtt_ScaleForPixelHeight(&info, (float)size);

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	font->ascent = (int)(ascent * scale);
	font->descent = (int)(descent * scale);
	font->line_height = (int)((abs(ascent) + abs(descent)) * scale);

	int area;
	load_characters(font, scale, &area, &info);
	font->texture = create_texture(font, renderer, scale, area, &info);
	return font;
}

FontCharacter font_get_char(Font* font, char c) {
	return font->characters[c - FIRST_CHARACTER];
}

void font_delete(Font* font, Renderer* renderer) {
	texture_delete(font->texture, renderer);
	m_free(font->texture, sizeof(Texture));
	m_free(font->data, font->data_size);
}