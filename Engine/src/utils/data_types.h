#pragma once
#include "eng_common_types.h"
#include "str_type.h"

typedef struct Array {
	size_t size;
	uint count;
	uint cap;
	void* data;
} Array;

typedef struct dic_entry dic_entry;

typedef struct Dictionary {
	size_t entry_count;
	size_t entry_size;
	dic_entry** entries;
} Dictionary;

typedef struct Stack {
	size_t size;
	size_t count;
	size_t cap;
	void* data;
} Stack;