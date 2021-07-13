#pragma once
#include "assets/material.h"
#include "assets/mesh.h"
#include "ecs/component/transform.h"

typedef struct BatchRenderer {
	Shader* shader;
	Material* material;
	Mesh mesh;
	void* vertices;
	uint vertices_count;
	size_t vertex_size;
	Texture** textures;
	uint textures_count;
} BatchRenderer;

BatchRenderer* batch_renderer_create(BatchRenderer* batch_renderer, Material* material, ADataType* layout, uint layout_size, size_t vertex_size);
void batch_renderer_delete(BatchRenderer* batch_renderer);

void batch_renderer_clear(BatchRenderer* batch_renderer);
void batch_renderer_add(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*));
void batch_renderer_add_sub(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec2i pos, vec2i size, void* data, void(*add_vertex)(void*, vec3, vec2, int, void*));
void batch_renderer_submit(BatchRenderer* batch_renderer);
void batch_renderer_draw(Transform* transform, BatchRenderer* batch_renderer, mat4* view_projection);