#pragma once
#include "assets/mesh.h"
#include "assets/material.h"
#include "ecs/component/transform.h"

typedef struct InstanceComponent {
	Mesh* mesh;
	Material* material;
	mat4* transforms;
	uint transforms_size;
	uint transforms_count;
} InstanceComponent;

InstanceComponent instance_component_create(Mesh* mesh, Material* material, uint transforms_count);
void instance_component_delete(InstanceComponent* instance_component);

void instance_component_add(InstanceComponent* instance_component, Transform* transform);