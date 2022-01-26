#pragma once
#include "eng_common_types.h"

typedef struct dic_entry dic_entry;

typedef struct Dictionary {
	size_t entry_count;
	size_t entry_size;
	dic_entry** entries;
} Dictionary;

Dictionary* dic_create(size_t entry_count, size_t entry_size);
void dic_delete(Dictionary* dic, void(*func)(void*));
void dic_delete_arg(Dictionary* dic, void(*func)(void*,void*), void* arg);

void* dic_add(Dictionary* dic, const char* key);
void* dic_get(Dictionary* dic, const char* key);
void dic_exec(Dictionary* dic, void(*func)(void*));