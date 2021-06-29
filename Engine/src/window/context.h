#pragma once
#include "api/ctx/acontext.h"
#include "window.h"

typedef struct Context {
	AContext* context;
} Context;

Context* context_create(Context* context, Window* window);
void context_delete(Context* context);

void context_swap_buffers(Context* context);