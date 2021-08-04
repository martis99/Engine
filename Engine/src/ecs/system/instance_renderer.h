#pragma once
#include "types.h"

InstanceRenderer* instance_renderer_create(InstanceRenderer* instance_renderer, Renderer* renderer);
void instance_renderer_delete(InstanceRenderer* instance_renderer, Ecs* ecs);

void instance_renderer_render(InstanceRenderer* instance_renderer, Ecs* ecs);

Material* instance_renderer_create_material(InstanceRenderer* instance_renderer, Assets* assets, const char* name, Texture* texture, vec4 color);