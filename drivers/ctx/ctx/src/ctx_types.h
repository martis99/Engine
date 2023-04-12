#pragma once
#include "api/ctx_api_types.h"

typedef struct CtxDriver {
	void *(*ctx_create)(void *context, void *window);
	void (*ctx_delete)(void *context);
	void (*ctx_swap_buffers)(void *context);
} CtxDriver;

#define CONTEXT_SIZE 256

typedef struct Context {
	CtxDriver *driver;
	const char *driver_str;
	byte context[CONTEXT_SIZE];
} Context;
