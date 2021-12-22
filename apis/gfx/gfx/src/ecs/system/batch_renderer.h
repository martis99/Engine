#pragma once
#include "gfx_types.h"

BatchRenderer* batch_renderer_create(BatchRenderer* batch_renderer, Renderer* renderer, Material* material);
void batch_renderer_delete(BatchRenderer* batch_renderer);

void batch_renderer_begin(BatchRenderer* batch_renderer);
void batch_renderer_add(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*));
void batch_renderer_add_sub(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec2i pos, vec2i size, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*));
void batch_renderer_end(Transform* transform, BatchRenderer* batch_renderer);