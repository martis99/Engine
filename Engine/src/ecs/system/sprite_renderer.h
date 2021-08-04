#pragma once
#include "types.h"

SpriteRenderer* sprite_renderer_create(SpriteRenderer* sprite_renderer, Renderer* renderer, Transform transform);
void sprite_renderer_delete(SpriteRenderer* sprite_renderer);

void sprite_renderer_render(SpriteRenderer* sprite_renderer, Ecs* ecs);