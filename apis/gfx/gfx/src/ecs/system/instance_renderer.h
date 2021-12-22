#pragma once
#include "gfx_types.h"

InstanceRenderer* instance_renderer_create(InstanceRenderer* instance_renderer, Renderer* renderer);
void instance_renderer_delete(InstanceRenderer* instance_renderer, Ecs* ecs);

void instance_renderer_render(InstanceRenderer* instance_renderer, Ecs* ecs);