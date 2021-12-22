#include "mesh_component.h"

MeshComponent mesh_component_create(Mesh* mesh, Material* material) {
	return (MeshComponent) { mesh, material };
}