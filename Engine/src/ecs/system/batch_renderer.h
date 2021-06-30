#pragma once
#include "assets/material.h"
#include "assets/mesh.h"
#include "ecs/component/transform.h"

typedef struct Vertex {
	vec3 position;
	vec4 color;
	vec2 tex_coord;
	float tex_index;
	vec2 size;
	vec4 borders;
} Vertex;

typedef struct BatchRenderer {
	Shader* shader;
	Material* material;
	Mesh mesh;
	Vertex* vertices;
	uint vertices_count;
	Texture** textures;
	uint textures_count;
} BatchRenderer;

BatchRenderer* batch_renderer_create(BatchRenderer* batch_renderer, Material* material);
void batch_renderer_delete(BatchRenderer* batch_renderer);

void batch_renderer_clear(BatchRenderer* batch_renderer);
void batch_renderer_add(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec4 color, vec4 borders);
void batch_renderer_add_sub(BatchRenderer* batch_renderer, Transform* transform, Texture* texture, vec4 color, vec2i pos, vec2i size);
void batch_renderer_submit(BatchRenderer* batch_renderer);
void batch_renderer_draw(Transform* transform, BatchRenderer* batch_renderer, mat4* view_projection);