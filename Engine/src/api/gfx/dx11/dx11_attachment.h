#pragma once
#ifdef GAPI_DX11
#include "dx11_atypes.h"

DX11Attachment* dx11_attachment_create(ARenderer* renderer, AAttachmentDesc desc, UINT width, UINT height);
void dx11_attachment_delete(DX11Attachment* attachment);

void dx11_attachment_srv_bind(DX11Attachment* attachment, ID3D11DeviceContext* context, UINT slot);
void dx11_attachment_srv_unbind(DX11Attachment* attachment, ID3D11DeviceContext* context, UINT slot);
void dx11_attachment_clear(DX11Attachment* attachment, ID3D11DeviceContext* context, const void* value);
void dx11_attachment_read_pixel(DX11Attachment* attachment, ID3D11DeviceContext* context, int x, int y, void* pixel);
#endif