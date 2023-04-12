#include "ctx_driver.h"

#include "ctx_none_types.h"

void *ctx_create(void *vcontext, void *vwindow)
{
	return vcontext;
}

void ctx_delete(void *vcontext)
{
}

void ctx_swap_buffers(void *context)
{
}

static CtxDriver driver = {
	.ctx_create	  = ctx_create,
	.ctx_delete	  = ctx_delete,
	.ctx_swap_buffers = ctx_swap_buffers,
};

CTX_DRIVER(NONE, 0, driver)
