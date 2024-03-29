#include "ctx_context.h"

#include "ctx_driver.h"

Context *context_create(Context *context, void *window, const char *driver)
{
	context->driver = ctx_driver_get(driver);
	if (context->driver == NULL) {
		log_error("CTX Driver not found");
		return NULL;
	}

	context->driver->ctx_create(context->context, window);
	context->driver_str = driver;
	return context;
}

void context_delete(Context *context)
{
	context->driver->ctx_delete(context->context);
}

void context_swap_buffers(Context *context)
{
	context->driver->ctx_swap_buffers(context->context);
}
