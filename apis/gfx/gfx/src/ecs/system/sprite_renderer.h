#pragma once
#include "gfx_types.h"

SpriteRenderer* sprite_renderer_create(SpriteRenderer* sprite_renderer, Renderer* renderer, Transform transform);
void sprite_renderer_delete(SpriteRenderer* sprite_renderer);

void sprite_renderer_begin(SpriteRenderer* sprite_renderer);
void sprite_renderer_render(SpriteRenderer* sprite_renderer, int id, Transform* transform, Sprite* sprite);
void sprite_renderer_end(SpriteRenderer* sprite_renderer);