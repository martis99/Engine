#pragma once
#include "types.h"

TextRenderer* text_renderer_create(TextRenderer* text_renderer, Renderer* renderer, Transform transform);
void text_renderer_delete(TextRenderer* text_renderer);

void text_renderer_render(TextRenderer* text_renderer, Ecs* ecs);

void text_renderer_calculate_preffered(Ecs* ecs);