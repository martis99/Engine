#include "pch.h"

Array* arr_create(Array* arr, size_t size, uint cap) {
	arr->size = size;
	arr->count = 0;
	arr->cap = cap;
	arr->data = m_malloc(cap * size);
	return arr;
}

void arr_delete(Array* arr, void(*func)(void*)) {
	arr_exec(arr, func);
	m_free(arr->data, arr->cap * arr->size);
}

void* arr_add(Array* arr) {
	return (byte*)arr->data + arr->count++ * arr->size;
}

void* arr_get(const Array* arr, uint index) {
	return (byte*)arr->data + index * arr->size;
}

void arr_exec(const Array* arr, void(*func)(void*)) {
	for (uint i = 0; i < arr->count; i++) {
		func(arr_get(arr, i));
	}
}