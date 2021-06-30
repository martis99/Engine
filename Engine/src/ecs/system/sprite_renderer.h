#pragma once
#include "batch_renderer.h"
#include "ecs/component/sprite.h"
#include "assets/assets.h"

typedef struct SpriteRenderer {
	Transform transform;
	BatchRenderer batch_renderer;
} SpriteRenderer;

SpriteRenderer* sprite_renderer_create(SpriteRenderer* sprite_renderer, Assets* assets, Transform transform);
void sprite_renderer_delete(SpriteRenderer* sprite_renderer);

void sprite_renderer_submit(SpriteRenderer* sprite_renderer);
void sprite_renderer_render(SpriteRenderer* sprite_renderer, Ecs* ecs, mat4* view_projection);