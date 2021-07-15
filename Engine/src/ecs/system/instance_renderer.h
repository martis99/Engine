#pragma once
#include "assets/assets.h"

typedef struct InstanceRenderer {
	Shader* shader;
} InstanceRenderer;

InstanceRenderer* instance_renderer_create(InstanceRenderer* mesh_renderer, Assets* assets);
void instance_renderer_delete(InstanceRenderer* mesh_renderer, Ecs* ecs);

void instance_renderer_render(InstanceRenderer* mesh_renderer, Ecs* ecs);

Material* instance_renderer_create_material(InstanceRenderer* mesh_renderer, Assets* assets, const char* name, Texture* texture, vec4 color);