#include "ctx/ctx_driver.h"

#include "gfx_none_types.h"

typedef struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
} AWindow;

AContext *ctx_create(void *window, LogCallbacks *log)
{
	AWindow *awindow  = window;
	AContext *context = m_malloc(sizeof(AContext));
	context->window	  = awindow->window;
	context->device	  = GetDC(context->window);
	return context;
}

void ctx_delete(AContext *context)
{
	ReleaseDC(context->window, context->device);
	m_free(context, sizeof(AContext));
}

void ctx_swap_buffers(AContext *context)
{
}

static CtxDriver driver = {
	.ctx_create	  = ctx_create,
	.ctx_delete	  = ctx_delete,
	.ctx_swap_buffers = ctx_swap_buffers,
};

CTX_DRIVER(NONE, driver)
