#include "api/gfx/gfx_api_mesh.h"
#include "dx11/dx11_buffer.h"

#include "gfx_dx11_types.h"

static ID3D11Buffer* create_vertex_buffer(ARenderer* renderer, AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	mesh->vertex_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->vertices = dx11_vb_create_dynamic(renderer->error, renderer->device, desc->max_count * mesh->vertex_size, mesh->vertex_size);
		mesh->vertices_count = 0;
	} else {
		mesh->vertices = dx11_vb_create_static(renderer->error, renderer->device, data.data, data.size, mesh->vertex_size);
		mesh->vertices_count = data.size / mesh->vertex_size;
	}
	return mesh->vertices;
}

static ID3D11Buffer* create_instance_buffer(ARenderer* renderer, AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	mesh->instance_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->instances = dx11_vb_create_dynamic(renderer->error, renderer->device, desc->max_count * mesh->instance_size, mesh->instance_size);
		mesh->instances_count = 0;
	} else {
		mesh->instances = dx11_vb_create_static(renderer->error, renderer->device, data.data, data.size, mesh->instance_size);
		mesh->instances_count = data.size / mesh->instance_size;
	}
	return mesh->instances;
}

static ID3D11Buffer* create_index_buffer(ARenderer* renderer, AMesh* mesh, ABufferDesc* desc, ABufferData data) {
	mesh->index_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->indices = dx11_ib_create_dynamic(renderer->error, renderer->device, desc->max_count * mesh->index_size, mesh->index_size);
		mesh->indices_count = 0;
	} else {
		mesh->indices = dx11_ib_create_static(renderer->error, renderer->device, data.data, data.size, mesh->index_size);
		mesh->indices_count = data.size / mesh->index_size;
	}
	mesh->index_format = dx11_atype_format(desc->values[0].type);
	return mesh->indices;
}

static void add_layout(D3D11_INPUT_ELEMENT_DESC* ied, ABufferDesc* desc, UINT* index, UINT slot, D3D11_INPUT_CLASSIFICATION slot_class, UINT step) {
	UINT layout_count = desc->values_size / sizeof(AValue);
	UINT aligment = 0;
	for (UINT i = 0; i < layout_count; i++) {
		for (uint j = 0; j < atype_count(desc->values[i].type, 0); j++) {
			ied[(*index)++] = (D3D11_INPUT_ELEMENT_DESC){ desc->values[i].name, j, dx11_atype_format(desc->values[i].type), slot, aligment, slot_class, step };
			aligment = D3D11_APPEND_ALIGNED_ELEMENT;
		}
	}
}

AMesh* amesh_create(ARenderer* renderer, AShader* shader, AShaderDesc desc, AMeshData data, APrimitive primitive) {
	AMesh* mesh = m_malloc(sizeof(AMesh));

	ABufferDesc* vertices_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN0);
	ABufferDesc* instances_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN1);
	ABufferDesc* indices_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_INDEX);

	UINT num_elements = (vertices_desc == NULL ? 0 : abufferdesc_count(vertices_desc, 0)) + (instances_desc == NULL ? 0 : abufferdesc_count(instances_desc, 0));
	D3D11_INPUT_ELEMENT_DESC* ied = m_malloc(num_elements * sizeof(D3D11_INPUT_ELEMENT_DESC));

	UINT index = 0;

	if (vertices_desc != NULL) {
		if (create_vertex_buffer(renderer, mesh, vertices_desc, data.vertices) == NULL) {
			log_msg(renderer->log, "Failed to create vertex buffer");
			return NULL;
		}
		add_layout(ied, vertices_desc, &index, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}

	if (instances_desc != NULL) {
		if (create_instance_buffer(renderer, mesh, instances_desc, data.instances) == NULL) {
			log_msg(renderer->log, "Failed to create instance buffer");
			return NULL;
		}
		add_layout(ied, instances_desc, &index, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	}

	if (indices_desc != NULL) {
		if (create_index_buffer(renderer, mesh, indices_desc, data.indices) == NULL) {
			log_msg(renderer->log, "Failed to create indices buffer");
			return NULL;
		}
	}

	mesh->layout = dx11_il_create(renderer->error, renderer->device, ied, num_elements, shader->vs_blob);
	if (mesh->layout == NULL) {
		log_msg(renderer->log, "Failed to create input layout");
		return NULL;
	}
	m_free(ied, num_elements * sizeof(D3D11_INPUT_ELEMENT_DESC));

	mesh->primitive = dx11_aprimitive(primitive);

	return mesh;
}

void amesh_delete(AMesh* mesh, ARenderer* renderer) {
	if (mesh->vertices != NULL) {
		dx11_vb_delete(mesh->vertices);
		mesh->vertices = NULL;
	}
	if (mesh->instances != NULL) {
		dx11_vb_delete(mesh->instances);
		mesh->instances = NULL;
	}
	if (mesh->indices != NULL) {
		dx11_ib_delete(mesh->indices);
		mesh->indices = NULL;
	}
	if (mesh->layout != NULL) {
		dx11_il_delete(mesh->layout);
		mesh->layout = NULL;
	}
	m_free(mesh, sizeof(AMesh));
}

void amesh_set_vertices(AMesh* mesh, ARenderer* renderer, const void* vertices, uint vertices_size) {
	if (mesh->vertices == NULL) {
		return;
	}
	dx11_vb_set_data(renderer->error, mesh->vertices, renderer->context, vertices, vertices_size);
	mesh->vertices_count = vertices_size / mesh->vertex_size;
}

void amesh_set_instances(AMesh* mesh, ARenderer* renderer, const void* instances, uint instances_size) {
	if (mesh->instances == NULL) {
		return;
	}
	dx11_vb_set_data(renderer->error, mesh->instances, renderer->context, instances, instances_size);
	mesh->instances_count = instances_size / mesh->instance_size;
}

void amesh_set_indices(AMesh* mesh, ARenderer* renderer, const void* indices, uint indices_size) {
	if (mesh->indices == NULL) {
		return;
	}
	dx11_ib_set_data(renderer->error, mesh->indices, renderer->context, indices, indices_size);
	mesh->indices_count = indices_size / mesh->index_size;
}

void amesh_draw(AMesh* mesh, ARenderer* renderer, uint indices) {
	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, mesh->primitive);
	dx11_il_bind(mesh->layout, renderer->context);

	if (mesh->indices == NULL) {
		if (mesh->instances == NULL) {
			dx11_vb_bind(mesh->vertices, renderer->context, mesh->vertex_size, 0);
			renderer->context->lpVtbl->Draw(renderer->context, mesh->vertices_count, 0);
		} else {
			ID3D11Buffer* buffers[] = { mesh->vertices, mesh->instances };
			UINT strides[] = { mesh->vertex_size, mesh->instance_size };
			UINT offsets[] = { 0, 0 };
			renderer->context->lpVtbl->IASetVertexBuffers(renderer->context, 0, 2, buffers, strides, offsets);

			renderer->context->lpVtbl->DrawInstanced(renderer->context, mesh->vertices_count, mesh->instances_count, 0, 0);
		}
	} else {
		DXGI_FORMAT frm = mesh->index_format;
		dx11_ib_bind(mesh->indices, renderer->context, mesh->index_format, 0);
		if (mesh->instances == NULL) {
			dx11_vb_bind(mesh->vertices, renderer->context, mesh->vertex_size, 0);
			renderer->context->lpVtbl->DrawIndexed(renderer->context, indices == 0xFFFFFFFF ? mesh->indices_count : indices, 0, 0);
		} else {
			ID3D11Buffer* buffers[] = { mesh->vertices, mesh->instances };
			UINT strides[] = { mesh->vertex_size, mesh->instance_size };
			UINT offsets[] = { 0, 0 };
			renderer->context->lpVtbl->IASetVertexBuffers(renderer->context, 0, 2, buffers, strides, offsets);

			renderer->context->lpVtbl->DrawIndexedInstanced(renderer->context, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->instances_count, 0, 0, 0);
		}
	}
}