#include "pch.h"
#include "context.h"
#include "api/ctx/acontext.h"

Context* context_create(Context* context, void* window) {
	context->context = acontext_create(window);
	return context;
}

void context_delete(Context* context) {
	acontext_delete(context->context);
}

void context_swap_buffers(Context* context) {
	acontext_swap_buffers(context->context);
}