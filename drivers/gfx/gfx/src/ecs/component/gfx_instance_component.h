#pragma once
#include "gfx/gfx_types.h"

InstanceComponent instance_component_create(Mesh *mesh, Material *material, uint transforms_count);
void instance_component_delete(InstanceComponent *instance_component);

void instance_component_add(InstanceComponent *instance_component, Transform *transform);
