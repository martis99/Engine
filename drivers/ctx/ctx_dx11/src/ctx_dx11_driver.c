#include "ctx_driver.h"

#include "ctx_dx11_types.h"
#include "dx11_ctx/dx11_error.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

typedef struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
} AWindow;

static void *ctx_create(void *vcontext, void *vwindow)
{
	AContext *context = vcontext;
	AWindow *window	  = vwindow;

	if (sizeof(AContext) > CONTEXT_SIZE) {
		log_error("context is too large: %zu", sizeof(AContext));
		return context;
	}

	context->window = window->window;

	DXGI_SWAP_CHAIN_DESC sd = {
		.BufferDesc = {
			.Width			 = 0,
			.Height			 = 0,
			.Format			 = DXGI_FORMAT_B8G8R8A8_UNORM,
			.RefreshRate.Numerator	 = 0,
			.RefreshRate.Denominator = 0,
			.Scaling		 = DXGI_MODE_SCALING_UNSPECIFIED,
			.ScanlineOrdering	 = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		},
		.SampleDesc = {
			.Count			 = 1,
			.Quality		 = 0,
		},
		.BufferUsage			 = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount			 = 2,
		.OutputWindow			 = context->window,
		.Windowed			 = TRUE,
		.SwapEffect			 = DXGI_SWAP_EFFECT_SEQUENTIAL,
		.Flags				 = 0,
	};

	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (dx11_error_create(&context->error) == NULL) {
		return NULL;
	}

	if (DX11_FAILED(&context->error, "Failed to create device and swapchain",
			D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &sd, &context->swap_chain,
						      &context->device, NULL, &context->context))) {
		return NULL;
	}

	return context;
}

static void ctx_delete(void *vcontext)
{
	AContext *context = vcontext;

#ifdef _DEBUG
	dx11_error_delete(&context->error);
#endif

	if (context->swap_chain != NULL) {
		context->swap_chain->lpVtbl->Release(context->swap_chain);
	}
	if (context->context != NULL) {
		context->context->lpVtbl->Flush(context->context);
		context->context->lpVtbl->Release(context->context);
	}

	if (context->device != NULL) {
		context->device->lpVtbl->Release(context->device);
	}
}

static void ctx_swap_buffers(void *vcontext)
{
	AContext *context = vcontext;

	if (DX11_FAILED(&context->error, "Failed to present", context->swap_chain->lpVtbl->Present(context->swap_chain, 0, 0))) {
		if (DX11_FAILED(&context->error, "Device removed reason", context->device->lpVtbl->GetDeviceRemovedReason(context->device))) {
			return;
		}
		return;
	}
}

static CtxDriver driver = {
	.ctx_create	  = ctx_create,
	.ctx_delete	  = ctx_delete,
	.ctx_swap_buffers = ctx_swap_buffers,
};

CTX_DRIVER(DX11, 0, driver)
