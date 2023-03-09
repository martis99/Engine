#include "ctx_driver.h"

#include "ctx_none_types.h"

typedef struct AWindow {
	byte id;
} AWindow;

AContext *ctx_create(void *window, LogCallbacks *log)
{
	AWindow *awindow  = window;
	AContext *context = m_malloc(sizeof(AContext));
	return context;
}

void ctx_delete(AContext *context)
{
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
