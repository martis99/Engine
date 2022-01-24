#include "api/gfx/gfx_api_mesh.h"
#include "gl/gl_buffer.h"

#include "gfx_gl_types.h"

static GLuint create_vertex_buffer(ARenderer* renderer, AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	mesh->vertex_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->vertices = gl_vb_create_dynamic(renderer->error, desc->max_count * mesh->vertex_size);
		mesh->vertices_count = 0;
	} else {
		mesh->vertices = gl_vb_create_static(renderer->error, data.data, data.size);
		mesh->vertices_count = data.size / mesh->vertex_size;
	}
	return mesh->vertices;
}

static GLuint create_instance_buffer(ARenderer* renderer, AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	mesh->instance_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->instances = gl_vb_create_dynamic(renderer->error, desc->max_count * mesh->instance_size);
		mesh->instances_count = 0;
	} else {
		mesh->instances = gl_vb_create_static(renderer->error, data.data, data.size);
		mesh->instances_count = data.size / mesh->instance_size;
	}
	return mesh->instances;
}

static GLuint create_index_buffer(ARenderer* renderer, AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	mesh->index_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->indices = gl_ib_create_dynamic(renderer->error, desc->max_count * mesh->index_size);
		mesh->indices_count = 0;
	} else {
		mesh->indices = gl_ib_create_static(renderer->error, data.data, data.size);
		mesh->indices_count = data.size / mesh->index_size;
	}
	mesh->index_type = gl_atype_type(desc->values[0].type);
	return mesh->indices;
}

static bool add_layout(ARenderer* renderer, ABufferDesc* desc, uint* index, GLuint divisor) {
	uint layout_count = desc->values_size / sizeof(AValue);

	GLuint stride = 0;
	for (uint i = 0; i < layout_count; i++) {
		stride += atype_size(desc->values[i].type);
	}

	byte offset = 0;
	for (uint i = 0; i < layout_count; i++) {
		for (uint j = 0; j < atype_count(desc->values[i].type, 0); j++) {
			if (gl_vertex_attrib(renderer->error, *index, atype_components(desc->values[i].type, 0), gl_atype_type(desc->values[i].type), stride, (void*)(offset), divisor) == A_FAIL) {
				return A_FAIL;
			}
			offset += atype_size(desc->values[i].type == MAT4F ? VEC4F : desc->values[i].type);
			(*index)++;
		}
	}

	return A_SUCCESS;
}

AMesh* amesh_create(ARenderer* renderer, AShader* shader, AShaderDesc desc, AMeshData data, APrimitive primitive) {
	AMesh* mesh = m_malloc(sizeof(AMesh));

	ABufferDesc* vertices_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN0);
	ABufferDesc* instances_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN1);
	ABufferDesc* indices_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_INDEX);

	mesh->va = gl_va_create(renderer->error);
	uint index = 0;
	if (vertices_desc != NULL) {
		if (create_vertex_buffer(renderer, mesh, vertices_desc, data.vertices) == 0) {
			log_msg(renderer->log, "Failed to create vertex buffer");
			return NULL;
		}
		if (add_layout(renderer, vertices_desc, &index, 0) == A_FAIL) {
			log_msg(renderer->log, "Failed to add vertex layout");
			return NULL;
		}
	}
	if (instances_desc != NULL) {
		if (create_instance_buffer(renderer, mesh, instances_desc, data.instances) == 0) {
			log_msg(renderer->log, "Failed to create instance buffer");
			return NULL;
		}
		if (add_layout(renderer, instances_desc, &index, 1) == A_FAIL) {
			log_msg(renderer->log, "Failed to add instance layout");
			return NULL;
		}
	}
	if (indices_desc != NULL) {
		if (create_index_buffer(renderer, mesh, indices_desc, data.indices) == 0) {
			log_msg(renderer->log, "Failed to create index buffer");
			return NULL;
		}
	}

	mesh->primitive = gl_aprimitive(primitive);

	return mesh;
}

void amesh_delete(AMesh* mesh, ARenderer* renderer) {
	if (mesh->va != 0) {
		gl_va_delete(renderer->error, mesh->va);
		mesh->va = 0;
	}
	if (mesh->vertices != 0) {
		gl_vb_delete(renderer->error, mesh->vertices);
		mesh->vertices = 0;
	}
	if (mesh->instances != 0) {
		gl_vb_delete(renderer->error, mesh->instances);
		mesh->instances = 0;
	}
	if (mesh->indices != 0) {
		gl_ib_delete(renderer->error, mesh->indices);
		mesh->indices = 0;
	}
	m_free(mesh, sizeof(AMesh));
}

void amesh_set_vertices(AMesh* mesh, ARenderer* renderer, const void* vertices, uint vertices_size) {
	gl_vb_set_data(renderer->error, mesh->vertices, vertices, vertices_size);
	mesh->vertices_count = vertices_size / mesh->vertex_size;
}

void amesh_set_instances(AMesh* mesh, ARenderer* renderer, const void* instances, uint instances_size) {
	gl_vb_set_data(renderer->error, mesh->instances, instances, instances_size);
	mesh->instances_count = instances_size / mesh->instance_size;
}

void amesh_set_indices(AMesh* mesh, ARenderer* renderer, const void* indices, uint indices_size) {
	gl_ib_set_data(renderer->error, mesh->indices, indices, indices_size);
	mesh->vertices_count = indices_size / mesh->index_size;
}

void amesh_draw(AMesh* mesh, ARenderer* renderer, uint indices) {
	gl_va_bind(renderer->error, mesh->va);
	if (mesh->indices == 0) {
		if (mesh->instances == 0) {
			gl_draw_arrays(renderer->error, mesh->primitive, mesh->vertices_count);
		} else {
			gl_draw_arrays_instanced(renderer->error, mesh->primitive, mesh->vertices_count, mesh->instances_count);
		}
	} else {
		gl_ib_bind(renderer->error, mesh->indices);
		if (mesh->instances == 0) {
			gl_draw_elements(renderer->error, mesh->primitive, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->index_type);
		} else {
			gl_draw_elements_instanced(renderer->error, mesh->primitive, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->index_type, mesh->instances_count);
		}
	}
}