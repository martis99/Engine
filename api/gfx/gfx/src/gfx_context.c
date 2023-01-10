#include "gfx_context.h"
#include "api/ctx/gfx_api_context.h"

Context *context_create(Context *context, void *window, LogCallbacks *log)
{
	context->context = acontext_create(window, log);
	return context;
}

void context_delete(Context *context)
{
	acontext_delete(context->context);
}

void context_swap_buffers(Context *context)
{
	acontext_swap_buffers(context->context);
}
