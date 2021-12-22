#pragma once
#include "data_types.h"

Stack* stack_create(size_t size);
void stack_delete(Stack* stack);

void* stack_pop(Stack* stack);
void stack_push(Stack* stack, void* data);