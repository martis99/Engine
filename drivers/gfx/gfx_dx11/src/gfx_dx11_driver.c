#include "gfx_driver.h"

#include "gfx_dx11_types.h"

#include "dx11/dx11_buffer.h"
#include "dx11/dx11_renderer.h"
#include "dx11/dx11_shader.h"
#include "dx11/dx11_texture.h"
#include "dx11_ctx/dx11_error.h"

#include "math/mat4.h"
#include "utils/str.h"

#include <stdio.h>

typedef union RasterizerStates {
	ID3D11RasterizerState *states[8];
	struct {
		ID3D11RasterizerState *cw_solid, *cw_solid_cull, *cw_wire, *cw_wire_cull;
		ID3D11RasterizerState *ccw_solid, *ccw_solid_cull, *ccw_wire, *ccw_wire_cull;
	};

} RasterizerStates;

typedef struct ARenderer {
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	AContext *acontext;
	ID3D11DepthStencilState *depth_stencil;
	ID3D11DepthStencilState *depth_stencil_depth;
	ID3D11DepthStencilState *depth_stencil_stencil;
	ID3D11DepthStencilState *depth_stencil_depth_stencil;
	ID3D11BlendState *blend_enabled;
	ID3D11BlendState *blend_disabled;
	RasterizerStates raster_states;
	DX11Error *error;
	int lhc;
	int cull_back;
} ARenderer;

typedef struct AShader {
	ID3DBlob *vs_blob;
	ID3D11VertexShader *vs;
	ID3DBlob *ps_blob;
	ID3D11PixelShader *ps;
} AShader;

typedef struct AMesh {
	UINT vertex_size;
	ID3D11Buffer *vertices;
	UINT vertices_count;

	UINT instance_size;
	ID3D11Buffer *instances;
	UINT instances_count;

	UINT index_size;
	ID3D11Buffer *indices;
	UINT indices_count;
	DXGI_FORMAT index_format;

	ID3D11InputLayout *layout;
	D3D11_PRIMITIVE_TOPOLOGY primitive;
} AMesh;

typedef struct ATexture {
	ID3D11Texture2D *texture;
	ID3D11ShaderResourceView *srv;
	ID3D11SamplerState *ss;
} ATexture;

typedef struct DX11Attachment {
	AType type;
	ID3D11Texture2D *texture;
	ID3D11RenderTargetView *rtv;
	ID3D11ShaderResourceView *srv;
	ID3D11SamplerState *ss;
	ID3D11Texture2D *st;
	uint pixel_size;
} DX11Attachment;

typedef struct AFramebuffer {
	ID3D11RenderTargetView *rtv;

	uint attachments_count;
	DX11Attachment *attachments;

	ID3D11Texture2D *dst;
	ID3D11DepthStencilView *dsv;

	byte shader[SHADER_SIZE];
	byte mesh[MESH_SIZE];
} AFramebuffer;

typedef struct AUniformBuffer {
	ID3D11Buffer *buffer;
	UINT slot;
} AUniformBuffer;

static D3D11_PRIMITIVE_TOPOLOGY dx11_aprimitive(APrimitive primitive)
{
	switch (primitive) {
	case A_POINTS: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	case A_LINES: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case A_TRIANGLES: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
	return 0;
}

static D3D11_TEXTURE_ADDRESS_MODE dx11_awrap(AWrap wrap)
{
	switch (wrap) {
	case A_REPEAT: return D3D11_TEXTURE_ADDRESS_WRAP;
	case A_CLAMP_TO_EDGE: return D3D11_TEXTURE_ADDRESS_CLAMP;
	}
	return 0;
}

static D3D11_COMPARISON_FUNC dx11_adepth_func(ADepthFunc func)
{
	switch (func) {
	case A_DEPTH_NEVER: return D3D11_COMPARISON_NEVER;
	case A_DEPTH_LESS: return D3D11_COMPARISON_LESS;
	case A_DEPTH_EQUAL: return D3D11_COMPARISON_EQUAL;
	case A_DEPTH_LEQUAL: return D3D11_COMPARISON_LESS_EQUAL;
	case A_DEPTH_GRATER: return D3D11_COMPARISON_GREATER;
	case A_DEPTH_NOTEQUAL: return D3D11_COMPARISON_NOT_EQUAL;
	case A_DEPTH_GEQUAL: return D3D11_COMPARISON_GREATER_EQUAL;
	case A_DEPTH_ALWAYS: return D3D11_COMPARISON_ALWAYS;
	}
	return 0;
}

static DXGI_FORMAT dx11_atype_format(AType type)
{
	switch (type) {
	case VEC1B: return DXGI_FORMAT_R8_SINT;
	case VEC2B: return DXGI_FORMAT_R8G8_SINT;
	case VEC3B: return 0;
	case VEC4B: return DXGI_FORMAT_R8G8B8A8_SINT;
	case VEC1UB: return DXGI_FORMAT_R8_UINT;
	case VEC2UB: return DXGI_FORMAT_R8G8_UINT;
	case VEC3UB: return 0;
	case VEC4UB: return DXGI_FORMAT_R8G8B8A8_UINT;
	case VEC1S: return DXGI_FORMAT_R16_SINT;
	case VEC2S: return DXGI_FORMAT_R16G16_SINT;
	case VEC3S: return 0;
	case VEC4S: return DXGI_FORMAT_R16G16B16A16_SINT;
	case VEC1US: return DXGI_FORMAT_R16_UINT;
	case VEC2US: return DXGI_FORMAT_R16G16_UINT;
	case VEC3US: return 0;
	case VEC4US: return DXGI_FORMAT_R16G16B16A16_UINT;
	case VEC1I: return DXGI_FORMAT_R32_SINT;
	case VEC2I: return DXGI_FORMAT_R32G32_SINT;
	case VEC3I: return DXGI_FORMAT_R32G32B32_SINT;
	case VEC4I: return DXGI_FORMAT_R32G32B32A32_SINT;
	case VEC1UI: return DXGI_FORMAT_R32_UINT;
	case VEC2UI: return DXGI_FORMAT_R32G32_UINT;
	case VEC3UI: return DXGI_FORMAT_R32G32B32_UINT;
	case VEC4UI: return DXGI_FORMAT_R32G32B32A32_UINT;
	case VEC1F: return DXGI_FORMAT_R32_FLOAT;
	case VEC2F: return DXGI_FORMAT_R32G32_FLOAT;
	case VEC3F: return DXGI_FORMAT_R32G32B32_FLOAT;
	case VEC4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case VEC1D: return 0;
	case VEC2D: return 0;
	case VEC3D: return 0;
	case VEC4D: return 0;
	case MAT4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	return 0;
}

static DX11Attachment *dx11_attachment_create(DX11Attachment *attachment, void *vrenderer, AAttachmentDesc desc, UINT width, UINT height)
{
	ARenderer *renderer = vrenderer;
	attachment->type    = desc.type;

	DXGI_FORMAT format = dx11_atype_format(desc.type);

	attachment->texture = dx11_texture_create(renderer->error, renderer->device, 0, width, height, format, 1, 1, 0, NULL, 0);
	if (attachment->texture == NULL) {
		log_error("Failed to create texture");
		return NULL;
	}

	attachment->rtv = dx11_rtv_create(renderer->error, renderer->device, format, attachment->texture);
	if (attachment->rtv == NULL) {
		log_error("failed to create render target view");
		return NULL;
	}

	attachment->srv = dx11_srv_create(renderer->error, renderer->device, 0, format, attachment->texture);
	if (attachment->srv == NULL) {
		log_error("failed to create shader resource view");
		return NULL;
	}

	attachment->ss = dx11_ss_create(renderer->error, renderer->device, desc.filter, dx11_awrap(desc.wrap));
	if (attachment->ss == NULL) {
		log_error("failed to create sampler state");
		return NULL;
	}

	if (desc.readable == 0) {
		attachment->st = NULL;
	} else {
		attachment->st = dx11_texture_create(renderer->error, renderer->device, 0, width, height, format, 0, 0, 1, NULL, 0);
		if (attachment->st == NULL) {
			log_error("failed to create attachment");
			return NULL;
		}
	}

	attachment->pixel_size = atype_size(desc.type);

	return attachment;
}

static void dx11_attachment_delete(DX11Attachment *attachment)
{
	if (attachment->texture != NULL) {
		dx11_texture_delete(attachment->texture);
		attachment->texture = NULL;
	}
	if (attachment->rtv != NULL) {
		dx11_rtv_delete(attachment->rtv);
		attachment->rtv = NULL;
	}
	if (attachment->srv != NULL) {
		dx11_srv_delete(attachment->srv);
		attachment->srv = NULL;
	}
	if (attachment->ss != NULL) {
		dx11_ss_delete(attachment->ss);
		attachment->ss = NULL;
	}
	if (attachment->st != NULL) {
		dx11_texture_delete(attachment->st);
		attachment->st = NULL;
	}
}

static void dx11_attachment_srv_bind(DX11Attachment *attachment, ID3D11DeviceContext *context, UINT slot)
{
	dx11_srv_bind(attachment->srv, context, slot);
	dx11_ss_bind(attachment->ss, context, slot);
}

static void dx11_attachment_srv_unbind(DX11Attachment *attachment, ID3D11DeviceContext *context, UINT slot)
{
	dx11_srv_bind(NULL, context, slot);
	dx11_ss_bind(NULL, context, slot);
}

static void dx11_attachment_clear(DX11Attachment *attachment, ID3D11DeviceContext *context, const void *value)
{
	float fvalue[4] = { 0 };
	atype_convert(fvalue, value, attachment->type);
	context->lpVtbl->ClearRenderTargetView(context, attachment->rtv, fvalue);
}

static void dx11_attachment_read_pixel(void *vrenderer, DX11Attachment *attachment, ID3D11DeviceContext *context, int x, int y, void *pixel)
{
	ARenderer *renderer = vrenderer;

	context->lpVtbl->CopyResource(context, (ID3D11Resource *)attachment->st, (ID3D11Resource *)attachment->texture);
	dx11_texture_read_pixel(renderer->error, attachment->st, context, x, y, attachment->pixel_size, pixel);
}

static ID3D11RenderTargetView *create_back_buffer_attachment(void *vrenderer, ID3D11RenderTargetView **view)
{
	ARenderer *renderer = vrenderer;

	ID3D11Resource *back_buffer = NULL;
	if (DX11_FAILED(renderer->error, "Failed to get back buffer",
			renderer->acontext->swap_chain->lpVtbl->GetBuffer(renderer->acontext->swap_chain, 0, &IID_ID3D11Resource, (void **)(&back_buffer)))) {
		return NULL;
	}
	if (DX11_FAILED(renderer->error, "Failed to create render target view",
			renderer->device->lpVtbl->CreateRenderTargetView(renderer->device, back_buffer, NULL, view))) {
		return NULL;
	}
	back_buffer->lpVtbl->Release(back_buffer);

	return *view;
}

static ID3D11DepthStencilView *create_depth_stencil_attachment(void *vrenderer, UINT width, UINT height, ID3D11Texture2D **texture, ID3D11DepthStencilView **view)
{
	ARenderer *renderer = vrenderer;

	D3D11_TEXTURE2D_DESC td = {
		.Width		    = width,
		.Height		    = height,
		.MipLevels	    = 1,
		.ArraySize	    = 1,
		.Format		    = DXGI_FORMAT_D32_FLOAT,
		.SampleDesc.Count   = 1,
		.SampleDesc.Quality = 0,
		.Usage		    = D3D11_USAGE_DEFAULT,
		.BindFlags	    = D3D11_BIND_DEPTH_STENCIL,
	};

	if (DX11_FAILED(renderer->error, "Failed to create texture", renderer->device->lpVtbl->CreateTexture2D(renderer->device, &td, NULL, texture))) {
		return NULL;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {
		.Format		    = DXGI_FORMAT_D32_FLOAT,
		.ViewDimension	    = D3D11_DSV_DIMENSION_TEXTURE2D,
		.Texture2D.MipSlice = 0,
	};

	if (DX11_FAILED(renderer->error, "Failed to create depth stencil view",
			renderer->device->lpVtbl->CreateDepthStencilView(renderer->device, (ID3D11Resource *)(*texture), &dsvd, view))) {
		return NULL;
	}

	return *view;
}

static ID3D11Buffer *create_vertex_buffer(void *vrenderer, void *vmesh, ABufferDesc *desc, ABufferData data)
{
	ARenderer *renderer = vrenderer;
	AMesh *mesh	    = vmesh;

	mesh->vertex_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->vertices	     = dx11_vb_create_dynamic(renderer->error, renderer->device, desc->max_count * mesh->vertex_size, mesh->vertex_size);
		mesh->vertices_count = 0;
	} else {
		mesh->vertices	     = dx11_vb_create_static(renderer->error, renderer->device, data.data, data.size, mesh->vertex_size);
		mesh->vertices_count = data.size / mesh->vertex_size;
	}
	return mesh->vertices;
}

static ID3D11Buffer *create_instance_buffer(void *vrenderer, void *vmesh, ABufferDesc *desc, ABufferData data)
{
	ARenderer *renderer = vrenderer;
	AMesh *mesh	    = vmesh;

	mesh->instance_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->instances	      = dx11_vb_create_dynamic(renderer->error, renderer->device, desc->max_count * mesh->instance_size, mesh->instance_size);
		mesh->instances_count = 0;
	} else {
		mesh->instances	      = dx11_vb_create_static(renderer->error, renderer->device, data.data, data.size, mesh->instance_size);
		mesh->instances_count = data.size / mesh->instance_size;
	}
	return mesh->instances;
}

static ID3D11Buffer *create_index_buffer(void *vrenderer, void *vmesh, ABufferDesc *desc, ABufferData data)
{
	ARenderer *renderer = vrenderer;
	AMesh *mesh	    = vmesh;

	mesh->index_size = abufferdesc_size(desc);
	if (data.data == NULL) {
		mesh->indices	    = dx11_ib_create_dynamic(renderer->error, renderer->device, desc->max_count * mesh->index_size, mesh->index_size);
		mesh->indices_count = 0;
	} else {
		mesh->indices	    = dx11_ib_create_static(renderer->error, renderer->device, data.data, data.size, mesh->index_size);
		mesh->indices_count = data.size / mesh->index_size;
	}
	mesh->index_format = dx11_atype_format(desc->values[0].type);
	return mesh->indices;
}

static void add_layout(D3D11_INPUT_ELEMENT_DESC *ied, ABufferDesc *desc, UINT *index, UINT slot, D3D11_INPUT_CLASSIFICATION slot_class, UINT step)
{
	UINT layout_count = desc->values_size / sizeof(AValue);
	UINT aligment	  = 0;
	for (UINT i = 0; i < layout_count; i++) {
		for (uint j = 0; j < atype_count(desc->values[i].type, 0); j++) {
			ied[(*index)++] =
				(D3D11_INPUT_ELEMENT_DESC){ desc->values[i].name, j, dx11_atype_format(desc->values[i].type), slot, aligment, slot_class, step };
			aligment = D3D11_APPEND_ALIGNED_ELEMENT;
		}
	}
}

static void *mesh_create(void *vmesh, void *vrenderer, void *vshader, AShaderDesc desc, AMeshData data, APrimitive primitive)
{
	AMesh *mesh	    = vmesh;
	ARenderer *renderer = vrenderer;
	AShader *shader	    = vshader;

	if (sizeof(AMesh) > MESH_SIZE) {
		log_error("mesh is too large: %zu", sizeof(AMesh));
		return mesh;
	}

	ABufferDesc *vertices_desc  = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN0);
	ABufferDesc *instances_desc = ashaderdesc_get_bufferdesc(desc, A_BFR_VS_IN1);
	ABufferDesc *indices_desc   = ashaderdesc_get_bufferdesc(desc, A_BFR_INDEX);

	UINT num_elements = (vertices_desc == NULL ? 0 : abufferdesc_count(vertices_desc, 0)) + (instances_desc == NULL ? 0 : abufferdesc_count(instances_desc, 0));
	D3D11_INPUT_ELEMENT_DESC *ied = m_malloc(num_elements * sizeof(D3D11_INPUT_ELEMENT_DESC));

	UINT index = 0;

	if (vertices_desc != NULL) {
		if (create_vertex_buffer(renderer, mesh, vertices_desc, data.vertices) == NULL) {
			log_error("failed to create vertex buffer");
		}
		add_layout(ied, vertices_desc, &index, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	} else {
		mesh->vertices	     = NULL;
		mesh->vertices_count = 0;
	}

	if (instances_desc != NULL) {
		if (create_instance_buffer(renderer, mesh, instances_desc, data.instances) == NULL) {
			log_error("failed to create instance buffer");
		}
		add_layout(ied, instances_desc, &index, 1, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	} else {
		mesh->instances	      = NULL;
		mesh->instances_count = 0;
	}

	if (indices_desc != NULL && data.indices.size > 0) {
		if (create_index_buffer(renderer, mesh, indices_desc, data.indices) == NULL) {
			log_error("failed to create indices buffer");
		}
	} else {
		mesh->indices	    = 0;
		mesh->indices_count = 0;
	}

	mesh->layout = dx11_il_create(renderer->error, renderer->device, ied, num_elements, shader->vs_blob);
	if (mesh->layout == NULL) {
		log_error("failed to create input layout");
	}
	m_free(ied, num_elements * sizeof(D3D11_INPUT_ELEMENT_DESC));

	mesh->primitive = dx11_aprimitive(primitive);

	return mesh;
}

static void mesh_delete(void *vmesh, void *vrenderer)
{
	AMesh *mesh = vmesh;

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
}

static void mesh_set_vertices(void *vmesh, void *vrenderer, const void *vertices, uint vertices_size)
{
	AMesh *mesh	    = vmesh;
	ARenderer *renderer = vrenderer;

	if (mesh->vertices == NULL) {
		return;
	}
	dx11_vb_set_data(renderer->error, mesh->vertices, renderer->context, vertices, vertices_size);
	mesh->vertices_count = vertices_size / mesh->vertex_size;
}

static void mesh_set_instances(void *vmesh, void *vrenderer, const void *instances, uint instances_size)
{
	AMesh *mesh	    = vmesh;
	ARenderer *renderer = vrenderer;

	if (mesh->instances == NULL) {
		return;
	}
	dx11_vb_set_data(renderer->error, mesh->instances, renderer->context, instances, instances_size);
	mesh->instances_count = instances_size / mesh->instance_size;
}

static void mesh_set_indices(void *vmesh, void *vrenderer, const void *indices, uint indices_size)
{
	AMesh *mesh	    = vmesh;
	ARenderer *renderer = vrenderer;

	if (mesh->indices == NULL) {
		return;
	}
	dx11_ib_set_data(renderer->error, mesh->indices, renderer->context, indices, indices_size);
	mesh->indices_count = indices_size / mesh->index_size;
}

static void mesh_draw(void *vmesh, void *vrenderer, uint indices)
{
	AMesh *mesh	    = vmesh;
	ARenderer *renderer = vrenderer;

	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, mesh->primitive);
	dx11_il_bind(mesh->layout, renderer->context);

	if (mesh->indices == NULL) {
		if (mesh->instances == NULL) {
			dx11_vb_bind(mesh->vertices, renderer->context, mesh->vertex_size, 0);
			renderer->context->lpVtbl->Draw(renderer->context, mesh->vertices_count, 0);
		} else {
			ID3D11Buffer *buffers[] = { mesh->vertices, mesh->instances };
			UINT strides[]		= { mesh->vertex_size, mesh->instance_size };
			UINT offsets[]		= { 0, 0 };
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
			ID3D11Buffer *buffers[] = { mesh->vertices, mesh->instances };
			UINT strides[]		= { mesh->vertex_size, mesh->instance_size };
			UINT offsets[]		= { 0, 0 };
			renderer->context->lpVtbl->IASetVertexBuffers(renderer->context, 0, 2, buffers, strides, offsets);

			renderer->context->lpVtbl->DrawIndexedInstanced(renderer->context, indices == 0xFFFFFFFF ? mesh->indices_count : indices, mesh->instances_count,
									0, 0, 0);
		}
	}
}

static void *renderer_create(void *vrenderer, void *vcontext, int lhc)
{
	ARenderer *renderer = vrenderer;
	AContext *context   = vcontext;

	if (sizeof(ARenderer) > RENDERER_SIZE) {
		log_error("renderer is too large: %zu", sizeof(ARenderer));
		return renderer;
	}

	renderer->device   = context->device;
	renderer->context  = context->context;
	renderer->acontext = context;
	renderer->error	   = &context->error;
	renderer->lhc	   = lhc;

	D3D11_VIEWPORT vp = {
		.Width	  = 1600,
		.Height	  = 900,
		.MinDepth = 0,
		.MaxDepth = 1,
		.TopLeftX = 0,
		.TopLeftY = 0,
	};

	renderer->context->lpVtbl->RSSetViewports(renderer->context, 1, &vp);

	D3D11_COMPARISON_FUNC depth = dx11_adepth_func(lhc == 1 ? A_DEPTH_LEQUAL : A_DEPTH_GEQUAL);
	renderer->depth_stencil	    = dx11_depth_stencil_create(&context->error, renderer->device, FALSE, FALSE, depth);
	if (renderer->depth_stencil == NULL) {
		log_error("failed to create depth stencil");
		return NULL;
	}

	renderer->depth_stencil_depth = dx11_depth_stencil_create(&context->error, renderer->device, TRUE, FALSE, depth);
	if (renderer->depth_stencil_depth == NULL) {
		log_error("failed to create depth stencil depth");
		return NULL;
	}

	renderer->depth_stencil_stencil = dx11_depth_stencil_create(&context->error, renderer->device, FALSE, TRUE, depth);
	if (renderer->depth_stencil_stencil == NULL) {
		log_error("failed to create depth stencil stencil");
		return NULL;
	}

	renderer->depth_stencil_depth_stencil = dx11_depth_stencil_create(&context->error, renderer->device, TRUE, TRUE, depth);
	if (renderer->depth_stencil_depth_stencil == NULL) {
		log_error("failed to create depth stencil depth stencil");
		return NULL;
	}

	for (int i = 0; i < 8; i++) {
		BOOL ccw	     = i / 4 % 2;
		D3D11_FILL_MODE fill = i / 2 % 2 == 0 ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
		D3D11_CULL_MODE cull = i % 2 == 0 ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		renderer->raster_states.states[i] = dx11_resterizer_create(&context->error, renderer->device, fill, cull, ccw);
		if (renderer->raster_states.cw_solid == NULL) {
			log_error("failed to create rasterizer");
			return NULL;
		}
	}

	renderer->blend_enabled = dx11_blend_create(&context->error, renderer->device, TRUE);
	if (renderer->blend_enabled == NULL) {
		log_error("failed to create blend enabled");
		return NULL;
	}

	renderer->blend_disabled = dx11_blend_create(&context->error, renderer->device, FALSE);
	if (renderer->blend_disabled == NULL) {
		log_error("failed to create blend disabled");
		return NULL;
	}

	return renderer;
}

static void renderer_delete(void *vrenderer)
{
	ARenderer *renderer = vrenderer;

	if (renderer->depth_stencil != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil);
		renderer->depth_stencil = NULL;
	}
	if (renderer->depth_stencil_depth != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil_depth);
		renderer->depth_stencil_depth = NULL;
	}
	if (renderer->depth_stencil_stencil != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil_stencil);
		renderer->depth_stencil_stencil = NULL;
	}
	if (renderer->depth_stencil_depth_stencil != NULL) {
		dx11_depth_stencil_delete(renderer->depth_stencil_depth_stencil);
		renderer->depth_stencil_depth_stencil = NULL;
	}
	for (int i = 0; i < 8; i++) {
		if (renderer->raster_states.states[i] != NULL) {
			dx11_rasterizer_delete(renderer->raster_states.states[i]);
			renderer->raster_states.states[i] = NULL;
		}
	}
	if (renderer->blend_enabled != NULL) {
		dx11_blend_delete(renderer->blend_enabled);
		renderer->blend_enabled = NULL;
	}
	if (renderer->blend_disabled != NULL) {
		dx11_blend_delete(renderer->blend_disabled);
		renderer->blend_disabled = NULL;
	}
}

static void renderer_depth_stencil_set(void *vrenderer, bool depth_enabled, bool stencil_enabled)
{
	ARenderer *renderer = vrenderer;

	if (depth_enabled == 0) {
		if (stencil_enabled == 0) {
			dx11_depth_stencil_bind(renderer->depth_stencil, renderer->context);
		} else {
			dx11_depth_stencil_bind(renderer->depth_stencil_stencil, renderer->context);
		}
	} else {
		if (stencil_enabled == 0) {
			dx11_depth_stencil_bind(renderer->depth_stencil_depth, renderer->context);
		} else {
			dx11_depth_stencil_bind(renderer->depth_stencil_depth_stencil, renderer->context);
		}
	}
}

static void renderer_rasterizer_set(void *vrenderer, bool wireframe, bool cull_back, bool ccw)
{
	ARenderer *renderer = vrenderer;

	renderer->cull_back = cull_back;
	dx11_rasterizer_bind(renderer->raster_states.states[ccw * 4 + wireframe * 2 + cull_back], renderer->context);
}

static void renderer_blend_set(void *vrenderer, bool enabled)
{
	ARenderer *renderer = vrenderer;

	if (enabled == 0) {
		dx11_blend_bind(renderer->blend_disabled, renderer->context);
	} else {
		dx11_blend_bind(renderer->blend_enabled, renderer->context);
	}
}

static mat4 renderer_perspective(void *vrenderer, float fovy, float aspect, float zNear, float zFar)
{
	ARenderer *renderer = vrenderer;

	return mat4_perspective0(fovy, aspect, zNear, zFar, renderer->lhc);
}

static mat4 renderer_ortho(void *vrenderer, float left, float right, float bottom, float top, float znear, float zfar)
{
	ARenderer *renderer = vrenderer;

	return mat4_ortho0(left, right, bottom, top, znear, zfar, renderer->lhc);
}

static float renderer_near(void *vrenderer)
{
	return 0;
}

static float renderer_far(void *vrenderer)
{
	return 1;
}

static void sg_generate(AShaderDesc desc, Str *vert, Str *frag)
{
	str_add_char(vert, '\0');

	str_add_char(frag, '\0');
}

static const char *sg_get_bnf()
{
	return "<program>                ::= { <vs_in_struct> | <vs_out_struct> | <fs_in_struct> | <fs_out_struct> | <buffer_struct> | <function_definition> | <function_prototype> | <textures_declaration> }1\n"
	       "<textures_declaration>   ::= 'Texture2D<float4> ' <identifier> '[' <int> '];' <new_line> 'SamplerState Samplers[' <int> '];' <new_line>\n"
	       "<vs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
	       "<vs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
	       "<fs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
	       "<fs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
	       "<buffer_struct>          ::= 'cbuffer ' <identifier> ' : register (b' <int> ') {' <new_line> { <tab> <type> ' ' <identifier> ';' <new_line> } '};' <new_line>\n"
	       "<struct_mem_sem>         ::= <tab> <type> ' ' <identifier> ' : ' <semantic> ';' <new_line> | <tab> <type> ' ' <identifier> ';' <new_line>\n"
	       "<semantic>               ::= <identifier>\n"
	       "<type>$i                 ::= 'void' | 'bool' | 'bool'  | 'int'   | 'int'    | 'int' | 'uint' | 'float' | 'double' | 'int1'  | 'int2'  | 'int3'  | 'int4'  | 'int1'   | 'int2'   | 'int3'   | 'int4'   | 'int1'  | 'int2'  | 'int3'  | 'int4'  | 'int1'   | 'int2'   | 'int3'   | 'int4'   | 'int1'  | 'int2'  | 'int3'  | 'int4'  | 'uint1'  | 'uint2'  | 'uint3'  | 'uint4'  | 'float1' | 'float2' | 'float3' | 'float4' | 'double1' | 'double2' | 'double3' | 'double4' | 'matrix'\n"
	       "<type_name>              ::= <type> | <identifier>\n"
	       "<function_prototype>     ::= <function_header> ';' <new_line>\n"
	       "<function_definition>    ::= <function_header> ' {' <new_line> { <tab> <compound> } '}' <new_line>\n"
	       "<function_header>        ::= <type_name> ' ' <identifier> '(' [ <parameters> ] ')'\n"
	       "<parameters>             ::= <parameter> ', ' <parameters> | <parameter>\n"
	       "<parameter>              ::= <type_name> ' ' <identifier>\n"
	       "<compound>               ::= <return_statement> | <if_statement> | <switch_statement> | <declaration_expression> | <assignment_expression> | <block> | <function_call>\n"
	       "<return_statement>       ::= 'return ' <expression> ';' <new_line>\n"
	       "<if_statement>           ::= 'if (' <expression> ') ' <block> ' else ' <if_statement> | 'if (' <expression> ') ' <block> ' else ' <compound> <new_line> | 'if (' <expression> ') ' <block> <new_line>\n"
	       "<block>                  ::= '{' <new_line> { <tab> <tab> <compound> } <tab> '}'\n"
	       "<switch_statement>       ::= 'switch (' <value> ') {' <new_line> { <tab> 'case ' <int> ': ' <compound> } <tab> '}' <new_line>\n"
	       "<declaration_expression> ::= <type_name> ' ' <identifier> [ <initialization> ] ';' <new_line>\n"
	       "<initialization>         ::= ' = ' <expression>\n"
	       "<assignment_expression>  ::= <lvalue> ' ' <assignment_operator> ' ' <expression> ';' <new_line>\n"
	       "<expression>             ::= <value> ' ' <expression_operator> ' ' <expression> | <paran_expression> ' ' <expression_operator> ' ' <expression> | <paran_expression> | <value>\n"
	       "<paran_expression>       ::= '(' <expression> ')'\n"
	       "<value>                  ::= <float> | <int> | <negative> | <lvalue> '++' | <lvalue> '--' | <function_call> '.' <value> | <array_access> '.' <value> | <identifier> '.' <value> | <function_call> | <array_access> | <identifier>\n"
	       "<array_access>           ::= <identifier> '[' <value> ']'\n"
	       "<negative>               ::= '-' <value>\n"
	       "<expression_operator>$v  ::= '+' | '-' | '*' | '/' | '<' | '>' | '==' | '>=' | '<='\n"
	       "<assignment_operator>$v  ::= '=' | '+=' | '-=' | '*=' | '/='\n"
	       "<lvalue>                 ::= <function_call> '.' <lvalue> | <array_access> '.' <lvalue>  | <identifier> '.' <lvalue> | <array_access> | <identifier>\n"
	       "<function_call>          ::= <mul_call> | <sample_call> | <discard_call> | <type> '(' [ <arguments> ] ')' | <identifier> '(' [ <arguments> ] ')'\n"
	       "<mul_call>               ::= 'mul(' <lexpression> ', ' <rexpression> ')'\n"
	       "<lexpression>            ::= <expression>\n"
	       "<rexpression>            ::= <expression>\n"
	       "<sample_call>            ::= <identifier> '[' <int> '].Sample(Samplers[' <int> '], ' <expression> ')'\n"
	       "<discard_call>           ::= 'discard;' <new_line>\n"
	       "<arguments>              ::= <expression> ', ' <arguments> | <expression>\n"
	       "\0";
}

static void *shader_create(void *vshader, void *vrenderer, const char *src_vert, const char *src_frag, const char *textures, uint num_textures)
{
	AShader *shader	    = vshader;
	ARenderer *renderer = vrenderer;

	if (sizeof(AShader) > SHADER_SIZE) {
		log_error("shader is too large: %zu", sizeof(AShader));
		return shader;
	}

	shader->vs = dx11_vs_create(renderer->error, renderer->device, src_vert, &shader->vs_blob);
	if (shader->vs == NULL) {
		log_error("failed to create vertex shader");
	}

	shader->ps = dx11_ps_create(renderer->error, renderer->device, src_frag, &shader->ps_blob);
	if (shader->ps == NULL) {
		log_error("failed to create pixel shader");
	}

	return shader;
}

static void shader_delete(void *vshader, void *vrenderer)
{
	AShader *shader = vshader;

	if (shader->vs != NULL && shader->vs_blob != NULL) {
		dx11_vs_delete(shader->vs, shader->vs_blob);
		shader->vs	= NULL;
		shader->vs_blob = NULL;
	}
	if (shader->ps != NULL && shader->ps_blob != NULL) {
		dx11_ps_delete(shader->ps, shader->ps_blob);
		shader->ps	= NULL;
		shader->ps_blob = NULL;
	}
}

static void shader_bind(void *vshader, void *vrenderer)
{
	AShader *shader	    = vshader;
	ARenderer *renderer = vrenderer;

	dx11_vs_bind(shader->vs, renderer->context);
	dx11_ps_bind(shader->ps, renderer->context);
}

static void *texture_create(void *vtexture, void *vrenderer, AWrap wrap, AFilter filter, int width, int height, int channels, void *data)
{
	ATexture *texture   = vtexture;
	ARenderer *renderer = vrenderer;

	if (sizeof(ATexture) > TEXTURE_SIZE) {
		log_error("texture is too large: %zu", sizeof(ATexture));
		return texture;
	}

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	switch (channels) {
	case 1: format = DXGI_FORMAT_R8_UNORM; break;
	case 2: format = DXGI_FORMAT_R8G8_UNORM; break;
	case 4: format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	}

	texture->texture = dx11_texture_create(renderer->error, renderer->device, filter, width, height, format, 0, 1, 0, data,
					       (unsigned long long)width * channels * sizeof(unsigned char));
	if (texture->texture == NULL) {
		log_error("failed to create texture");
		return NULL;
	}

	if (dx11_mipmap_afilter(filter) == 1) {
		dx11_texture_update_subresource(renderer->context, texture->texture, data, (unsigned long long)width * channels * sizeof(unsigned char), height);
	}

	texture->srv = dx11_srv_create(renderer->error, renderer->device, filter, format, texture->texture);
	if (texture->srv == NULL) {
		log_error("failed to create shader resource view");
		return NULL;
	}

	if (dx11_mipmap_afilter(filter) == 1) {
		dx11_srv_generate_mips(texture->srv, renderer->context);
	}

	texture->ss = dx11_ss_create(renderer->error, renderer->device, filter, dx11_awrap(wrap));
	if (texture->ss == NULL) {
		log_error("failed to create sampler state");
		return NULL;
	}

	return texture;
}

static void texture_bind(void *vtexture, void *vrenderer, uint slot)
{
	ATexture *texture   = vtexture;
	ARenderer *renderer = vrenderer;

	dx11_srv_bind(texture->srv, renderer->context, slot);
	dx11_ss_bind(texture->ss, renderer->context, slot);
}

static void texture_delete(void *vtexture, void *vrenderer)
{
	ATexture *texture = vtexture;

	if (texture->texture != NULL) {
		dx11_texture_delete(texture->texture);
		texture->texture = NULL;
	}
	if (texture->srv != NULL) {
		dx11_srv_delete(texture->srv);
		texture->srv = NULL;
	}
	if (texture->ss != NULL) {
		dx11_ss_delete(texture->ss);
		texture->ss = NULL;
	}
}

static void *ub_create_static(void *vuniform_buffer, void *vrenderer, uint slot, uint data_size, const void *data)
{
	AUniformBuffer *uniform_buffer = vuniform_buffer;
	ARenderer *renderer	       = vrenderer;

	if (sizeof(AUniformBuffer) > UNIFORM_BUFFER_SIZE) {
		log_error("uniform buffer is too large: %zu", sizeof(AUniformBuffer));
		return uniform_buffer;
	}

	uniform_buffer->buffer = dx11_cb_create_static(renderer->error, renderer->device, data, data_size);

	if (uniform_buffer->buffer == NULL) {
		log_error("failed to create static constant buffer");
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

static void *ub_create_dynamic(void *vuniform_buffer, void *vrenderer, uint slot, uint data_size)
{
	AUniformBuffer *uniform_buffer = vuniform_buffer;
	ARenderer *renderer	       = vrenderer;

	if (sizeof(AUniformBuffer) > UNIFORM_BUFFER_SIZE) {
		log_error("uniform buffer is too large: %zu", sizeof(AUniformBuffer));
		return uniform_buffer;
	}

	uniform_buffer->buffer = dx11_cb_create_dynamic(renderer->error, renderer->device, data_size);

	if (uniform_buffer->buffer == NULL) {
		log_error("failed to create dynamic constant buffer");
	}
	uniform_buffer->slot = slot;
	return uniform_buffer;
}

static void ub_delete(void *vuniform_buffer, void *vrenderer)
{
	AUniformBuffer *uniform_buffer = vuniform_buffer;

	if (uniform_buffer->buffer != NULL) {
		dx11_cb_delete(uniform_buffer->buffer);
		uniform_buffer->buffer = NULL;
	}
}

static void ub_upload(void *vuniform_buffer, void *vrenderer, const void *data, uint data_size)
{
	AUniformBuffer *uniform_buffer = vuniform_buffer;
	ARenderer *renderer	       = vrenderer;

	dx11_cb_set_data(renderer->error, uniform_buffer->buffer, renderer->context, data, data_size);
}

static void ub_bind_vs(void *vuniform_buffer, void *vrenderer)
{
	AUniformBuffer *uniform_buffer = vuniform_buffer;
	ARenderer *renderer	       = vrenderer;

	dx11_cb_bind_vs(uniform_buffer->buffer, renderer->context, uniform_buffer->slot);
}

static void ub_bind_ps(void *vuniform_buffer, void *vrenderer)
{
	AUniformBuffer *uniform_buffer = vuniform_buffer;
	ARenderer *renderer	       = vrenderer;

	dx11_cb_bind_ps(uniform_buffer->buffer, renderer->context, uniform_buffer->slot);
}

static void *fb_create(void *vframebuffer, void *vrenderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	if (sizeof(AFramebuffer) > FRAMEBUFFER_SIZE) {
		log_error("framebuffer is too large: %zu", sizeof(AFramebuffer));
		return framebuffer;
	}

	if (create_back_buffer_attachment(renderer, &framebuffer->rtv) == NULL) {
		log_error("failed to create back buffer attachment");
	}

	framebuffer->attachments_count = attachments_size / sizeof(AAttachmentDesc);
	framebuffer->attachments       = m_malloc(framebuffer->attachments_count * sizeof(DX11Attachment));

	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		dx11_attachment_create(&framebuffer->attachments[i], renderer, attachments[i], width, height);
	}

	if (create_depth_stencil_attachment(renderer, width, height, &framebuffer->dst, &framebuffer->dsv) == NULL) {
		log_error("failed to create depth stencil attachment");
	}

	const char *src_vert = "struct Input {\n"
			       "	float3 pos         : Position;\n"
			       "	float2 tex_coord   : TexCoord;\n"
			       "};\n"
			       "struct Output {\n"
			       "	float4 pos         : SV_Position;\n"
			       "	float2 tex_coord   : TexCoord;\n"
			       "};\n"
			       "Output vs_main(Input input) {\n"
			       "	Output output;\n"
			       "	output.pos         = float4(input.pos, 1.0f);\n"
			       "	output.tex_coord   = input.tex_coord;\n"
			       "	return output;\n"
			       "}\0";

	const char *src_frag = "Texture2D Texture;\n"
			       "SamplerState Sampler;\n"
			       "struct Input {\n"
			       "	float4 pos         : SV_Position;\n"
			       "	float2 tex_coord   : TexCoord;\n"
			       "};\n"
			       "float4 fs_main(Input input) : SV_TARGET {\n"
			       "	return Texture.Sample(Sampler, input.tex_coord);\n"
			       "}\0";

	shader_create(framebuffer->shader, renderer, src_vert, src_frag, "Texture", 1);

	AValue vertex[] = {
		{ VEC3F, "Position" },
		{ VEC2F, "TexCoord" },
	};

	AValue index[] = {
		{ VEC1UI, "" },
	};

	AValue output[] = {
		{ VEC4F, "Color" },
	};

	ABufferDesc buffers[] = {
		{ A_BFR_VS_IN0, 0, vertex, sizeof(vertex), 0, "Input" },
		{ A_BFR_INDEX, 0, index, sizeof(index), 0, "" },
		{ A_BFR_PS_OUT, 0, output, sizeof(output), 0, "Output" },
	};

	AShaderDesc shader_desc = {
		.buffers	= buffers,
		.buffers_size	= sizeof(buffers),
		.textures_count = 1,
		.texture_type	= VEC4F,
	};

	// clang-format off
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	};

	uint indices[] = {
		0, 1, 2,
		0, 2, 3,
	};
	// clang-format on

	AMeshData md = {
		.vertices.data = vertices,
		.vertices.size = sizeof(vertices),
		.indices.data  = indices,
		.indices.size  = sizeof(indices),
	};

	mesh_create((AMesh *)framebuffer->mesh, renderer, framebuffer->shader, shader_desc, md, A_TRIANGLES);

	return framebuffer;
}

static void fb_delete(void *vframebuffer, void *vrenderer)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	if (framebuffer->rtv != NULL) {
		framebuffer->rtv->lpVtbl->Release(framebuffer->rtv);
		framebuffer->rtv = NULL;
	}

	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		dx11_attachment_delete(&framebuffer->attachments[i]);
	}
	m_free(framebuffer->attachments, framebuffer->attachments_count * sizeof(DX11Attachment));

	if (framebuffer->dst != NULL) {
		framebuffer->dst->lpVtbl->Release(framebuffer->dst);
		framebuffer->dst = NULL;
	}
	if (framebuffer->dsv != NULL) {
		framebuffer->dsv->lpVtbl->Release(framebuffer->dsv);
		framebuffer->dsv = NULL;
	}

	shader_delete(framebuffer->shader, renderer);
	mesh_delete(framebuffer->mesh, renderer);
}

static void fb_bind_render_targets(void *vframebuffer, void *vrenderer, uint *targets, uint targets_size)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	ID3D11RenderTargetView *rtvs[8] = { 0 };

	uint targets_count = targets_size / sizeof(uint);
	for (uint i = 0; i < targets_count; i++) {
		rtvs[i] = framebuffer->attachments[targets[i]].rtv;
	}
	renderer->context->lpVtbl->OMSetRenderTargets(renderer->context, targets_count, rtvs, framebuffer->dsv);
}

static void fb_unbind_render_targets(void *vframebuffer, void *vrenderer, uint *targets, uint targets_size)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	ID3D11RenderTargetView *rtvs[8] = { 0 };
	uint targets_count		= targets_size / sizeof(uint);
	renderer->context->lpVtbl->OMSetRenderTargets(renderer->context, targets_count, rtvs, NULL);
}

static void fb_clear_attachment(void *vframebuffer, void *vrenderer, uint id, const void *value)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	dx11_attachment_clear(&framebuffer->attachments[id], renderer->context, value);
}

static void fb_clear_depth_attachment(void *vframebuffer, void *vrenderer, const void *value)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	renderer->context->lpVtbl->ClearDepthStencilView(renderer->context, framebuffer->dsv, D3D11_CLEAR_DEPTH, renderer->lhc == 1 ? 1.0f : 0.0f, 0);
}

static void fb_read_pixel(void *vframebuffer, void *vrenderer, uint id, int x, int y, void *pixel)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	dx11_attachment_read_pixel(renderer, &framebuffer->attachments[1], renderer->context, x, y, pixel);
}

static void fb_draw(void *vframebuffer, void *vrenderer, uint id)
{
	AFramebuffer *framebuffer = vframebuffer;
	ARenderer *renderer	  = vrenderer;

	renderer->context->lpVtbl->OMSetRenderTargets(renderer->context, 1, &framebuffer->rtv, framebuffer->dsv);

	FLOAT color[] = { 1, 0, 0, 0 };
	renderer->context->lpVtbl->ClearRenderTargetView(renderer->context, framebuffer->rtv, color);
	FLOAT depth = 0.0f;
	renderer->context->lpVtbl->ClearDepthStencilView(renderer->context, framebuffer->dsv, D3D11_CLEAR_DEPTH, renderer->lhc == 1 ? 1.0f : 0.0f, 0);

	shader_bind(framebuffer->shader, renderer);
	dx11_attachment_srv_bind(&framebuffer->attachments[id], renderer->context, 0);
	mesh_draw(framebuffer->mesh, renderer, 0xFFFFFFFF);
	dx11_attachment_srv_unbind(&framebuffer->attachments[id], renderer->context, 0);
}

static GfxDriver driver = {
	.fb_create		    = fb_create,
	.fb_delete		    = fb_delete,
	.fb_clear_depth_attachment  = fb_clear_depth_attachment,
	.fb_clear_attachment	    = fb_clear_attachment,
	.fb_draw		    = fb_draw,
	.fb_read_pixel		    = fb_read_pixel,
	.fb_bind_render_targets	    = fb_bind_render_targets,
	.fb_unbind_render_targets   = fb_unbind_render_targets,
	.mesh_create		    = mesh_create,
	.mesh_set_indices	    = mesh_set_indices,
	.mesh_delete		    = mesh_delete,
	.mesh_draw		    = mesh_draw,
	.mesh_set_instances	    = mesh_set_instances,
	.mesh_set_vertices	    = mesh_set_vertices,
	.renderer_blend_set	    = renderer_blend_set,
	.renderer_create	    = renderer_create,
	.renderer_delete	    = renderer_delete,
	.renderer_depth_stencil_set = renderer_depth_stencil_set,
	.renderer_far		    = renderer_far,
	.renderer_near		    = renderer_near,
	.renderer_ortho		    = renderer_ortho,
	.renderer_perspective	    = renderer_perspective,
	.renderer_rasterizer_set    = renderer_rasterizer_set,
	.sg_generate		    = sg_generate,
	.sg_get_bnf		    = sg_get_bnf,
	.shader_bind		    = shader_bind,
	.shader_create		    = shader_create,
	.shader_delete		    = shader_delete,
	.texture_bind		    = texture_bind,
	.texture_create		    = texture_create,
	.texture_delete		    = texture_delete,
	.ub_bind_ps		    = ub_bind_ps,
	.ub_bind_vs		    = ub_bind_vs,
	.ub_create_dynamic	    = ub_create_dynamic,
	.ub_create_static	    = ub_create_static,
	.ub_delete		    = ub_delete,
	.ub_upload		    = ub_upload,
};

GFX_DRIVER(DX11, driver)
