#pragma once
#include "gfx_types.h"

InstanceRenderer* instance_renderer_create(InstanceRenderer* instance_renderer, Renderer* renderer);
void instance_renderer_delete(InstanceRenderer* instance_renderer);

void instance_renderer_begin(InstanceRenderer* instance_renderer);
void instance_renderer_render(InstanceRenderer* instance_renderer, int id, Transform* transform, InstanceComponent* instance_component);
void instance_renderer_end(InstanceRenderer* instance_renderer);