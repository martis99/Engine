#include "dx11_attachment.h"
#include "dx11/dx11_texture.h"

DX11Attachment* dx11_attachment_create(ARenderer* renderer, AAttachmentDesc desc, UINT width, UINT height) {
	DX11Attachment* attachment = m_malloc(sizeof(DX11Attachment));
	attachment->type = desc.type;

	DXGI_FORMAT format = dx11_atype_format(desc.type);

	attachment->texture = dx11_texture_create(renderer->device, width, height, format, 1, 1, 0, NULL, 0);
	if (attachment->texture == NULL) {
		log_error("Failed to create texture");
		return NULL;
	}

	attachment->rtv = dx11_rtv_create(renderer->device, format, attachment->texture);
	if (attachment->rtv == NULL) {
		log_error("Failed to create render target view");
		return NULL;
	}

	attachment->srv = dx11_srv_create(renderer->device, format, attachment->texture);
	if (attachment->srv == NULL) {
		log_error("Failed to create shader resource view");
		return NULL;
	}

	attachment->ss = dx11_ss_create(renderer->device, dx11_afilter(desc.filter), dx11_awrap(desc.wrap));
	if (attachment->ss == NULL) {
		log_error("Failed to create sampler state");
		return NULL;
	}

	if (desc.readable == 0) {
		attachment->st = NULL;
	} else {
		attachment->st = dx11_texture_create(renderer->device, width, height, format, 0, 0, 1, NULL, 0);
		if (attachment->st == NULL) {
			log_error("Failed to create attachment");
			return NULL;
		}
	}

	attachment->pixel_size = atype_size(desc.type);

	return attachment;
}

void dx11_attachment_delete(DX11Attachment* attachment) {
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
	m_free(attachment, sizeof(DX11Attachment));
}

void dx11_attachment_srv_bind(DX11Attachment* attachment, ID3D11DeviceContext* context, UINT slot) {
	dx11_srv_bind(attachment->srv, context, slot);
	dx11_ss_bind(attachment->ss, context, slot);
}

void dx11_attachment_srv_unbind(DX11Attachment* attachment, ID3D11DeviceContext* context, UINT slot) {
	dx11_srv_bind(NULL, context, slot);
	dx11_ss_bind(NULL, context, slot);
}

void dx11_attachment_clear(DX11Attachment* attachment, ID3D11DeviceContext* context, const void* value) {
	float fvalue[4] = { 0 };
	atype_convert(fvalue, value, attachment->type);
	context->lpVtbl->ClearRenderTargetView(context, attachment->rtv, fvalue);
}

void dx11_attachment_read_pixel(DX11Attachment* attachment, ID3D11DeviceContext* context, int x, int y, void* pixel) {
	context->lpVtbl->CopyResource(context, (ID3D11Resource*)attachment->st, (ID3D11Resource*)attachment->texture);
	dx11_texture_read_pixel(attachment->st, context, x, y, attachment->pixel_size, pixel);
}