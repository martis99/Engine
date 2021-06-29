#include "pch.h"

#include "app.h"

void* m_realloc(void* memory, size_t new_size, size_t old_size) {
	app.stats.memory -= old_size;
	app.stats.memory += new_size;
	return realloc(memory, new_size);
}

void* m_calloc(size_t count, size_t size) {
	app.stats.memory += count * size;
	return calloc(count, size);
}

void* m_malloc(size_t size) {
	app.stats.memory += size;
	return malloc(size);
}

void m_free(void* memory, size_t size) {
	app.stats.memory -= size;
	free(memory);
}