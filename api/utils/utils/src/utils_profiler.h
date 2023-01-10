#pragma once
void *utils_profiler_create();
void utils_profiler_delete();

void utils_profiler_start(const char *file);
void utils_profiler_end();
