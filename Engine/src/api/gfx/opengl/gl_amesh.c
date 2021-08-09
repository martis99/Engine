#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/amesh.h"
#include "gl_atypes.h"
#include "gl/gl_buffer.h"

static void add_layout(ABufferDesc* desc, uint* index, GLuint divisor) {
	uint layout_count = desc->values_size / sizeof(AValue);

	GLuint stride = 0;
	for (uint i = 0; i < layout_count; i++) {
		stride += atype_size(desc->values[i].type);
	}

	byte offset = 0;
	for (uint i = 0; i < layout_count; i++) {
		for (uint j = 0; j < atype_count(desc->values[i].type, 0); j++) {
			glEnableVertexAttribArray(*index);
			switch (desc->values[i].type) {
			case VEC1B:
			case VEC2B:
			case VEC3B:
			case VEC4B:
			case VEC1UB:
			case VEC2UB:
			case VEC3UB:
			case VEC4UB:
			case VEC1S:
			case VEC2S:
			case VEC3S:
			case VEC4S:
			case VEC1US:
			case VEC2US:
			case VEC3US:
			case VEC4US:
			case VEC1I:
			case VEC2I:
			case VEC3I:
			case VEC4I:
			case VEC1UI:
			case VEC2UI:
			case VEC3UI:
			case VEC4UI:
				glVertexAttribIPointer(*index, atype_components(desc->values[i].type, 0), gl_atype_type(desc->values[i].type), stride, (void*)(offset));
				offset += atype_size(desc->values[i].type);
				break;
			case VEC1F:
			case VEC2F:
			case VEC3F:
			case VEC4F:
				glVertexAttribPointer(*index, atype_components(desc->values[i].type, 0), gl_atype_type(desc->values[i].type), GL_FALSE, stride, (void*)(offset));
				offset += atype_size(desc->values[i].type);
				break;
			case VEC1D:
			case VEC2D:
			case VEC3D:
			case VEC4D:
				glVertexAttribLPointer(*index, atype_components(desc->values[i].type, 0), gl_atype_type(desc->values[i].type), stride, (void*)(offset));
				offset += atype_size(desc->values[i].type);
				break;
			case MAT4F:
				glVertexAttribPointer(*index, atype_components(desc->values[i].type, 0), gl_atype_type(desc->values[i].type), GL_FALSE, stride, (void*)(offset));
				offset += atype_size(VEC4F);
				break;
			}
			glVertexAttribDivisor(*index, divisor);
			(*index)++;
		}
	}
}

static void create_vertex_buffer(AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	if (desc == NULL) {
		return;
	}
	mesh->vertex_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->vertices = gl_vb_create_dynamic(desc->max_count * mesh->vertex_size);
		mesh->vertices_count = 0;
	} else {
		mesh->vertices = gl_vb_create_static(data.data, data.size);
		mesh->vertices_count = data.size / mesh->vertex_size;
	}
}

static void create_instance_buffer(AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	if (desc == NULL) {
		return;
	}
	mesh->instance_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->instances = gl_vb_create_dynamic(desc->max_count * mesh->instance_size);
		mesh->instances_count = 0;
	} else {
		mesh->instances = gl_vb_create_static(data.data, data.size);
		mesh->instances_count = data.size / mesh->instance_size;
	}
}

static void create_index_buffer(AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	if (desc == NULL) {
		return;
	}
	mesh->index_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->indices = gl_ib_create_dynamic(desc->max_count * mesh->index_size);
		mesh->indices_count = 0;
	} else {
		mesh->indices = gl_ib_create_static(data.data, data.size);
		mesh->indices_count = data.size / mesh->index_size;
	}
	mesh->index_type = gl_atype_type(desc->values[0].type);
}

AMesh* amesh_create(ARenderer* renderer, AShader* shader, AShaderDesc desc, AMeshData data, APrimitive primitive) {
	AMesh* mesh = m_malloc(sizeof(AMesh));

	ABufferDesc* vertices_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_VERTEX);
	ABufferDesc* instances_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_INSTANCE);
	ABufferDesc* indices_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_INDEX);

	mesh->va = gl_va_create();
	uint index = 0;
	create_vertex_buffer(mesh, vertices_desc, data.vertices);
	if (vertices_desc != NULL) {
		add_layout(vertices_desc, &index, 0);
	}

	create_instance_buffer(mesh, instances_desc, data.instances);
	if (instances_desc != NULL) {
		add_layout(instances_desc, &index, 1);
	}

	create_index_buffer(mesh, indices_desc, data.indices);

	mesh->primitive = gl_aprimitive(primitive);

	return mesh;
}

void amesh_delete(AMesh* mesh) {
	if (mesh->va != 0) {
		gl_va_delete(mesh->va);
	}
	if (mesh->vertices != 0) {
		gl_vb_delete(mesh->vertices);
	}
	if (mesh->instances != 0) {
		gl_vb_delete(mesh->instances);
	}
	if (mesh->indices != 0) {
		gl_ib_delete(mesh->indices);
	}
	m_free(mesh, sizeof(AMesh));
}

void amesh_set_vertices(AMesh* mesh, ARenderer* renderer, const void* vertices, uint vertices_size) {
	gl_vb_set_data(mesh->vertices, vertices, vertices_size);
	mesh->vertices_count = vertices_size / mesh->vertex_size;
}

void amesh_set_instances(AMesh* mesh, ARenderer* renderer, const void* instances, uint instances_size) {
	gl_vb_set_data(mesh->instances, instances, instances_size);
	mesh->instances_count = instances_size / mesh->instance_size;
}

void amesh_set_indices(AMesh* mesh, ARenderer* renderer, const void* indices, uint indices_size) {
	gl_ib_set_data(mesh->indices, indices, indices_size);
	mesh->vertices_count = indices_size / mesh->index_size;
}

void amesh_draw(AMesh* mesh, ARenderer* renderer, uint indices) {

	if (mesh->indices == 0) {
		if (mesh->instances == 0) {
			gl_va_draw_arrays(mesh->va, mesh->primitive, mesh->vertices_count);
		} else {
			gl_va_draw_arrays_instanced(mesh->va, mesh->primitive, mesh->vertices_count, mesh->instances_count);
		}
	} else {
		if (mesh->instances == 0) {
			gl_va_draw_elements(mesh->va, mesh->indices, mesh->primitive, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->index_type);
		} else {
			gl_va_draw_elements_instanced(mesh->va, mesh->indices, mesh->primitive, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->index_type, mesh->instances_count);
		}
	}
}

#endif