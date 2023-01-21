#pragma once
#include "ctx_types.h"

Context *context_create(Context *context, void *window, LogCallbacks *log, const char *driver);
void context_delete(Context *context);

void context_swap_buffers(Context *context);
