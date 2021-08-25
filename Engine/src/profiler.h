#pragma once
#include "types.h"

void* profiler_create();
void profiler_delete();

void profiler_start(const char* file);
void profiler_end();