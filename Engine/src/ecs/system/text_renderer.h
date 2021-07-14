#pragma once
#include "batch_renderer.h"
#include "ecs/component/text.h"
#include "assets/assets.h"

typedef struct TextRenderer {
	Transform transform;
	BatchRenderer batch_renderer;
} TextRenderer;

TextRenderer* text_renderer_create(TextRenderer* text_renderer, Assets* assets, Transform transform);
void text_renderer_delete(TextRenderer* text_renderer);

void text_renderer_submit(TextRenderer* text_renderer);
void text_renderer_render(TextRenderer* text_renderer, Ecs* ecs);

void text_renderer_calculate_preffered(Ecs* ecs);