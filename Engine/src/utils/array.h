#pragma once
#include "data_types.h"

Array* arr_create(Array* arr, size_t size, uint cap);
void arr_delete(Array* arr, void(*func)(void*));

void* arr_add(Array* arr);
void* arr_get(const Array* arr, uint index);
void arr_exec(const Array* arr, void(*func)(void*));