#pragma once
#include "data_types.h"

Dictionary* dic_create(size_t entry_count, size_t entry_size);
void dic_delete(Dictionary* dic, void(*func)(void*));

void* dic_add(Dictionary* dic, const char* key);
void* dic_get(Dictionary* dic, const char* key);
void dic_exec(Dictionary* dic, void(*func)(void*));