#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/amesh.h"
#include "gl_atypes.h"
#include "gl/gl_buffer.h"

static void add_layout(ABufferDesc desc, uint* index, GLuint divisor) {
	uint layout_count = desc.layout_size / sizeof(AValue);

	GLuint stride = 0;
	for (uint i = 0; i < layout_count; i++) {
		stride += atype_size(desc.layout[i].type);
	}

	byte offset = 0;
	for (uint i = 0; i < layout_count; i++) {
		for (uint j = 0; j < atype_count(desc.layout[i].type, 0); j++) {
			glEnableVertexAttribArray(*index);
			switch (desc.layout[i].type) {
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
				glVertexAttribIPointer(*index, atype_components(desc.layout[i].type, 0), gl_atype_type(desc.layout[i].type), stride, (void*)(offset));
				offset += atype_size(desc.layout[i].type);
				break;
			case VEC1F:
			case VEC2F:
			case VEC3F:
			case VEC4F:
				glVertexAttribPointer(*index, atype_components(desc.layout[i].type, 0), gl_atype_type(desc.layout[i].type), GL_FALSE, stride, (void*)(offset));
				offset += atype_size(desc.layout[i].type);
				break;
			case VEC1D:
			case VEC2D:
			case VEC3D:
			case VEC4D:
				glVertexAttribLPointer(*index, atype_components(desc.layout[i].type, 0), gl_atype_type(desc.layout[i].type), stride, (void*)(offset));
				offset += atype_size(desc.layout[i].type);
				break;
			case MAT4F:
				glVertexAttribPointer(*index, atype_components(desc.layout[i].type, 0), gl_atype_type(desc.layout[i].type), GL_FALSE, stride, (void*)(offset));
				offset += atype_size(VEC4F);
				break;
			}
			glVertexAttribDivisor(*index, divisor);
			(*index)++;
		}
	}
}

static GLuint create_vertex_buffer(AMesh* mesh, ABufferDesc desc, uint* index) {
	mesh->vertex_size = abufferdesc_size(desc);

	if (desc.data == NULL) {
		mesh->vertices = gl_vb_create_dynamic(desc.data_size);
		mesh->vertices_count = 0;
	} else {
		mesh->vertices = gl_vb_create_static(desc.data, desc.data_size);
		mesh->vertices_count = desc.data_size / mesh->vertex_size;
	}

	add_layout(desc, index, 0);
	return mesh->vertices;
}

static GLuint create_instance_buffer(AMesh* mesh, ABufferDesc desc, uint* index) {
	mesh->instance_size = abufferdesc_size(desc);

	if (desc.data == NULL) {
		mesh->instances = gl_vb_create_dynamic(desc.data_size);
		mesh->instances_count = 0;
	} else {
		mesh->instances = gl_vb_create_static(desc.data, desc.data_size);
		mesh->instances_count = desc.data_size / mesh->instance_size;
	}

	add_layout(desc, index, 1);
	return mesh->instances;
}

static GLuint create_index_buffer(AMesh* mesh, ABufferDesc desc) {
	mesh->index_size = abufferdesc_size(desc);

	if (desc.data == NULL) {
		mesh->indices = gl_ib_create_dynamic(desc.data_size);
		mesh->indices_count = 0;
	} else {
		mesh->indices = gl_ib_create_static(desc.data, desc.data_size);
		mesh->indices_count = desc.data_size / mesh->index_size;
	}

	mesh->index_type = gl_atype_type(desc.layout[0].type);
	return mesh->indices;
}

AMesh* amesh_create(ARenderer* renderer, AShader* shader, AMeshDesc desc, APrimitive primitive) {
	AMesh* mesh = m_malloc(sizeof(AMesh));

	mesh->va = gl_va_create();
	uint index = 0;

	if (desc.vertices.enabled == 0) {
		mesh->vertex_size = 0;
		mesh->vertices = 0;
		mesh->vertices_count = 0;
	} else {
		if (create_vertex_buffer(mesh, desc.vertices, &index) == 0) {
			log_error("Failed to create vertex buffer");
			return NULL;
		}
	}

	if (desc.instances.enabled == 0) {
		mesh->instance_size = 0;
		mesh->instances = 0;
		mesh->instances_count = 0;
	} else {
		if (create_instance_buffer(mesh, desc.instances, &index) == 0) {
			log_error("Failed to create instance buffer");
			return NULL;
		}
	}

	if (desc.indices.enabled == 0) {
		mesh->index_size = 0;
		mesh->indices = 0;
		mesh->indices_count = 0;
		mesh->index_type = 0;
	} else {
		if (create_index_buffer(mesh, desc.indices) == 0) {
			log_error("Failed to create index buffer");
			return NULL;
		}
	}

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