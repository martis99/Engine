#include "stack.h"

#include "mem.h"

#include <memory.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 4

Stack *stack_create(size_t size)
{
	Stack *stack = (Stack *)m_malloc(sizeof(Stack));
	if (stack == NULL) {
		return NULL;
	}
	stack->size  = size;
	stack->data  = m_malloc(size * INITIAL_CAPACITY);
	stack->cap   = INITIAL_CAPACITY;
	stack->count = 0;
	return stack;
}

void stack_delete(Stack *stack)
{
	m_free(stack->data, stack->size * stack->cap);
	m_free(stack, sizeof(Stack));
}

void *stack_pop(Stack *stack)
{
	byte *ret = (byte *)stack->data;
	ret += (--stack->count) * stack->size;
	return ret;
}

void stack_push(Stack *stack, void *data)
{
	if (stack->count == stack->cap) {
		void *data = m_realloc(stack->data, stack->cap * 2 * stack->size, stack->cap * stack->size);
		if (NULL == data) {
			printf("Failed to reallocate memory %s:%d\n", __FILE__, __LINE__);
			exit(1);
		} else {
			stack->data = data;
			stack->cap *= 2;
		}
	}
	byte *ptr = (byte *)stack->data;
	ptr += (stack->count++) * stack->size;
	memcpy(ptr, data, stack->size);
}
