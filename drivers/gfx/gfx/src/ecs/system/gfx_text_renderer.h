#pragma once
#include "gfx_types.h"

TextRenderer *text_renderer_create(TextRenderer *text_renderer, Renderer *renderer, Transform transform);
void text_renderer_delete(TextRenderer *text_renderer);

void text_renderer_begin(TextRenderer *text_renderer);
void text_renderer_render(TextRenderer *text_renderer, int id, Transform *transform, Text *text);
void text_renderer_end(TextRenderer *text_renderer);
