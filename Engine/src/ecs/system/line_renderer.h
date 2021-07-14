#pragma once
#include "assets/assets.h"
#include "assets/material.h"
#include "assets/mesh.h"
#include "ecs/component/transform.h"

typedef struct LineVertex {
	vec3 position;
	vec4 color;
	int entity;
} LineVertex;

typedef struct LineRenderer {
	Transform transform;
	Shader* shader;
	Mesh mesh;
	LineVertex* vertices;
	uint vertices_count;
} LineRenderer;

LineRenderer* line_renderer_create(LineRenderer* line_renderer, Assets* assets, Transform transform);
void line_renderer_delete(LineRenderer* line_renderer);

void line_renderer_clear(LineRenderer* line_renderer);
void line_renderer_add(LineRenderer* line_renderer, vec3 start, vec3 end, vec4 color, int entity);
void line_renderer_submit(LineRenderer* line_renderer);
void line_renderer_render(LineRenderer* line_renderer);