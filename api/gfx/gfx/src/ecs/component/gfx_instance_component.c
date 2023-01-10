#include "gfx_instance_component.h"

#include "ecs/component/transform.h"

InstanceComponent instance_component_create(Mesh *mesh, Material *material, uint transforms_count)
{
	return (InstanceComponent){
		.mesh		  = mesh,
		.material	  = material,
		.transforms	  = m_malloc(transforms_count * sizeof(mat4)),
		.transforms_size  = transforms_count,
		.transforms_count = 0,
	};
}

void instance_component_delete(InstanceComponent *instance_component)
{
	m_free(instance_component->transforms, instance_component->transforms_size * sizeof(mat4));
}

void instance_component_add(InstanceComponent *instance_component, Transform *transform)
{
	instance_component->transforms[instance_component->transforms_count++] = transform_to_mat4(transform);
}
