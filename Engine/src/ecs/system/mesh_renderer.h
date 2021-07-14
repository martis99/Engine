#pragma once
#include "assets/assets.h"

typedef struct MeshRenderer {
	Shader* shader;
} MeshRenderer;

MeshRenderer* mesh_renderer_create(MeshRenderer* mesh_renderer, Assets* assets);
void mesh_renderer_delete(MeshRenderer* mesh_renderer);

void mesh_renderer_render(MeshRenderer* mesh_renderer, Ecs* ecs);

Material* mesh_renderer_create_material(MeshRenderer* mesh_renderer, Assets* assets, const char* name, Texture* texture, vec4 color);