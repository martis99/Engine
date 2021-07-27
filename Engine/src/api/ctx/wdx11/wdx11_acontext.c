#include "pch.h"
#ifdef E_WINDOWS
#ifdef GAPI_DX11
#include "api/ctx/acontext.h"
#include "api/wnd/awindow.h"

#include <Windows.h>

#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxguid.lib")

struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
	AWindowCallbacks callbacks;
	ACursor* cursor;
};

struct AContext {
	HWND window;
	ID3D11Device* device;
	IDXGISwapChain* swap_chain;
	ID3D11DeviceContext* context;
};

AContext* acontext_create(AWindow* window) {
	AContext* context = m_malloc(sizeof(AContext));
	context->window = awindow_get_window(window);

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = context->window;
	sd.Windowed = TRUE;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&context->swap_chain,
		&context->device,
		NULL,
		&context->context
	);

	return context;
}

void acontext_delete(AContext* context) {
	if (context->context != NULL) {
		context->context->lpVtbl->Release(context->context);
	}
	if (context->swap_chain != NULL) {
		context->swap_chain->lpVtbl->Release(context->swap_chain);
	}
	if (context->device != NULL) {
		context->device->lpVtbl->Release(context->device);
	}
	m_free(context, sizeof(AContext));
}

void acontext_swap_buffers(AContext* context) {
	context->swap_chain->lpVtbl->Present(context->swap_chain, 1, 0);
}
#endif
#endif