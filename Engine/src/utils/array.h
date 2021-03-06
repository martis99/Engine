#pragma once
#include "eng_common_types.h"

typedef struct Array {
	size_t size;
	uint count;
	uint cap;
	void* data;
} Array;

Array* arr_create(Array* arr, size_t size, uint cap);
void arr_delete(Array* arr, void(*func)(void*));
void arr_delete_arg(Array* arr, void(*func)(void*, void*), void* arg);

void* arr_add(Array* arr);
void* arr_get(const Array* arr, uint index);
void arr_exec(const Array* arr, void(*func)(void*));
void arr_exec_arg(const Array* arr, void(*func)(void*, void*), void* arg);