#include "api/gfx/gfx_api_mesh.h"

#include "gfx_none_types.h"

AMesh *amesh_create(ARenderer *renderer, AShader *shader, AShaderDesc desc, AMeshData data, APrimitive primitive)
{
	AMesh *mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

void amesh_delete(AMesh *mesh, ARenderer *renderer)
{
	m_free(mesh, sizeof(AMesh));
}

void amesh_set_vertices(AMesh *mesh, ARenderer *renderer, const void *vertices, uint vertices_size)
{
}

void amesh_set_instances(AMesh *mesh, ARenderer *renderer, const void *instances, uint instances_size)
{
}

void amesh_set_indices(AMesh *mesh, ARenderer *renderer, const void *indices, uint indices_size)
{
}

void amesh_draw(AMesh *mesh, ARenderer *renderer, uint indices)
{
}
