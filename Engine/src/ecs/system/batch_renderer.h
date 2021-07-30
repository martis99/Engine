#pragma once
#include "structs.h"

BatchRenderer* batch_renderer_create(BatchRenderer* batch_renderer, Renderer* renderer, Material* material, size_t vertex_size);
void batch_renderer_delete(BatchRenderer* batch_renderer);

void batch_renderer_clear(BatchRenderer* batch_renderer);
void batch_renderer_add(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*));
void batch_renderer_add_sub(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec2i pos, vec2i size, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*));
void batch_renderer_submit(BatchRenderer* batch_renderer);
void batch_renderer_draw(Transform* transform, BatchRenderer* batch_renderer);