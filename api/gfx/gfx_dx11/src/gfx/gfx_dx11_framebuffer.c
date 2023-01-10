#include "api/gfx/gfx_api_framebuffer.h"
#include "api/gfx/gfx_api_mesh.h"
#include "api/gfx/gfx_api_shader.h"
#include "dx11/dx11_error.h"
#include "gfx_dx11_attachment.h"

static ID3D11RenderTargetView *create_back_buffer_attachment(ARenderer *renderer, ID3D11RenderTargetView **view)
{
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

static ID3D11DepthStencilView *create_depth_stencil_attachment(ARenderer *renderer, UINT width, UINT height, ID3D11Texture2D **texture, ID3D11DepthStencilView **view)
{
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

AFramebuffer *aframebuffer_create(ARenderer *renderer, AAttachmentDesc *attachments, uint attachments_size, int width, int height)
{
	AFramebuffer *framebuffer = m_malloc(sizeof(AFramebuffer));

	if (create_back_buffer_attachment(renderer, &framebuffer->rtv) == NULL) {
		log_msg(renderer->log, "Failed to create back buffer attachment");
		return NULL;
	}

	framebuffer->attachments_count = attachments_size / sizeof(AAttachmentDesc);
	framebuffer->attachments       = m_malloc(framebuffer->attachments_count * sizeof(DX11Attachment *));

	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		framebuffer->attachments[i] = dx11_attachment_create(renderer, attachments[i], width, height);
		if (framebuffer->attachments[i] == NULL) {
			log_msg(renderer->log, "Failed to create attachment");
			return NULL;
		}
	}

	if (create_depth_stencil_attachment(renderer, width, height, &framebuffer->dst, &framebuffer->dsv) == NULL) {
		log_msg(renderer->log, "Failed to create depth stencil attachment");
		return NULL;
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

	framebuffer->shader = ashader_create(renderer, src_vert, src_frag, "Texture", 1);
	if (framebuffer->shader == NULL) {
		log_msg(renderer->log, "Failed to create shader");
		return NULL;
	}

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

	framebuffer->mesh = amesh_create(renderer, framebuffer->shader, shader_desc, md, A_TRIANGLES);
	if (framebuffer->mesh == NULL) {
		log_msg(renderer->log, "Failed to create mesh");
		return NULL;
	}

	return framebuffer;
}

void aframebuffer_delete(AFramebuffer *framebuffer, ARenderer *renderer)
{
	if (framebuffer->rtv != NULL) {
		framebuffer->rtv->lpVtbl->Release(framebuffer->rtv);
		framebuffer->rtv = NULL;
	}

	for (uint i = 0; i < framebuffer->attachments_count; i++) {
		dx11_attachment_delete(framebuffer->attachments[i]);
	}
	m_free(framebuffer->attachments, framebuffer->attachments_count * sizeof(DX11Attachment *));

	if (framebuffer->dst != NULL) {
		framebuffer->dst->lpVtbl->Release(framebuffer->dst);
		framebuffer->dst = NULL;
	}
	if (framebuffer->dsv != NULL) {
		framebuffer->dsv->lpVtbl->Release(framebuffer->dsv);
		framebuffer->dsv = NULL;
	}

	ashader_delete(framebuffer->shader, renderer);
	amesh_delete(framebuffer->mesh, renderer);

	m_free(framebuffer, sizeof(AFramebuffer));
}

void aframebuffer_set_render_targets(AFramebuffer *framebuffer, ARenderer *renderer, uint *targets, uint targets_size)
{
	ID3D11RenderTargetView *rtvs[8] = { 0 };

	uint targets_count = targets_size / sizeof(uint);
	for (uint i = 0; i < targets_count; i++) {
		rtvs[i] = framebuffer->attachments[targets[i]]->rtv;
	}
	renderer->context->lpVtbl->OMSetRenderTargets(renderer->context, targets_count, rtvs, framebuffer->dsv);
}

void aframebuffer_clear_attachment(AFramebuffer *framebuffer, ARenderer *renderer, uint id, const void *value)
{
	dx11_attachment_clear(framebuffer->attachments[id], renderer->context, value);
}

void aframebuffer_clear_depth_attachment(AFramebuffer *framebuffer, ARenderer *renderer, const void *value)
{
	renderer->context->lpVtbl->ClearDepthStencilView(renderer->context, framebuffer->dsv, D3D11_CLEAR_DEPTH, renderer->lhc == 1 ? 1.0f : 0.0f, 0);
}

void aframebuffer_read_pixel(AFramebuffer *framebuffer, ARenderer *renderer, uint id, int x, int y, void *pixel)
{
	dx11_attachment_read_pixel(renderer, framebuffer->attachments[1], renderer->context, x, y, pixel);
}

void aframebuffer_draw(AFramebuffer *framebuffer, ARenderer *renderer, uint id)
{
	renderer->context->lpVtbl->OMSetRenderTargets(renderer->context, 1, &framebuffer->rtv, framebuffer->dsv);

	FLOAT color[] = { 1, 0, 0, 0 };
	renderer->context->lpVtbl->ClearRenderTargetView(renderer->context, framebuffer->rtv, color);
	FLOAT depth = 0.0f;
	renderer->context->lpVtbl->ClearDepthStencilView(renderer->context, framebuffer->dsv, D3D11_CLEAR_DEPTH, renderer->lhc == 1 ? 1.0f : 0.0f, 0);

	ashader_bind(framebuffer->shader, renderer);
	dx11_attachment_srv_bind(framebuffer->attachments[id], renderer->context, 0);
	amesh_draw(framebuffer->mesh, renderer, 0xFFFFFFFF);
	dx11_attachment_srv_unbind(framebuffer->attachments[id], renderer->context, 0);
}
