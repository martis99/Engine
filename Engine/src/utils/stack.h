#pragma once
#include "eng_common_types.h"

typedef struct Stack {
	size_t size;
	size_t count;
	size_t cap;
	void *data;
} Stack;

Stack *stack_create(size_t size);
void stack_delete(Stack *stack);

void *stack_pop(Stack *stack);
void stack_push(Stack *stack, void *data);
