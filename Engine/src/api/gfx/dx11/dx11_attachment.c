#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_attachment.h"
#include "dx11/dx11_texture.h"

DX11Attachment* dx11_attachment_create(ARenderer* renderer, AAttachmentDesc desc, UINT width, UINT height) {
	DX11Attachment* attachment = m_malloc(sizeof(DX11Attachment));
	attachment->type = desc.type;

	DXGI_FORMAT format = dx11_atype_format(desc.type);

	attachment->texture = dx11_texture_create(renderer->device, width, height, format, 1, 1, 0, NULL, 0);
	if (attachment->texture == NULL) {
		log_error("Failed to create attachment");
		return NULL;
	}

	attachment->rtv = dx11_rtv_create(renderer->device, format, attachment->texture);
	if (attachment->rtv == NULL) {
		log_error("Failed to create attachment");
		return NULL;
	}

	attachment->srv = dx11_srv_create(renderer->device, format, attachment->texture);
	if (attachment->srv == NULL) {
		log_error("Failed to create attachment");
		return NULL;
	}

	attachment->ss = dx11_ss_create(renderer->device, dx11_afilter(desc.filter), dx11_awrap(desc.wrap));
	if (attachment->ss == NULL) {
		log_error("Failed to create attachment");
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
	dx11_texture_delete(attachment->texture);
	dx11_rtv_delete(attachment->rtv);
	dx11_srv_delete(attachment->srv);
	dx11_ss_delete(attachment->ss);
	dx11_texture_delete(attachment->st);
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
	D3D11_MAPPED_SUBRESOURCE ms = { 0 };

	HRESULT hr = context->lpVtbl->Map(context, (ID3D11Resource*)attachment->st, 0, D3D11_MAP_READ, 0, &ms);
	if (FAILED(hr)) {
		log_error("Failed to map texture");
		return;
	}

	memcpy(pixel, (byte*)ms.pData + y * ms.RowPitch + x * attachment->pixel_size, attachment->pixel_size);

	context->lpVtbl->Unmap(context, (ID3D11Resource*)attachment->st, 0);
}
#endif