#pragma once
#include "api/ctx_api_types.h"

typedef struct CtxDriver {
	AContext *(*ctx_create)(void *window, LogCallbacks *log);
	void (*ctx_delete)(AContext *context);
	void (*ctx_swap_buffers)(AContext *context);
} CtxDriver;

typedef struct Context {
	AContext *context;
	CtxDriver *driver;
	const char *driver_str;
} Context;
