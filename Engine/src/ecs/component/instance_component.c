#include "pch.h"
#include "instance_component.h"

#include "assets/mesh.h"
#include "ecs/component/transform.h"

InstanceComponent instance_component_create(Mesh* mesh, Material* material, uint transforms_count) {
	InstanceComponent ic = { 0 };
	ic.mesh = mesh;
	ic.material = material;
	ic.transforms = m_malloc(transforms_count * sizeof(mat4));
	ic.transforms_size = transforms_count;
	ic.transforms_count = 0;

	AValue layout[] = {
		{"Model", MAT4F}
	};
	mesh_add_instance_buffer_dynamic(ic.mesh, ic.transforms_size * sizeof(mat4), layout, sizeof(layout));
	return ic;
}

void instance_component_delete(InstanceComponent* instance_component) {
	m_free(instance_component->transforms, instance_component->transforms_size * sizeof(mat4));
}

void instance_component_add(InstanceComponent* instance_component, Transform* transform) {
	instance_component->transforms[instance_component->transforms_count++] = transform_to_mat4(transform);
}