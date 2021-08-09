#include "pch.h"

#include "app.h"

void* m_realloc(void* memory, size_t new_size, size_t old_size) {
	app_get_stats()->memory -= old_size;
	app_get_stats()->memory += new_size;
	return realloc(memory, new_size);
}

void* m_calloc(size_t count, size_t size) {
	app_get_stats()->memory += count * size;
	return calloc(count, size);
}

void* m_malloc(size_t size) {
	app_get_stats()->memory += size;
	void* ptr = malloc(size);
	if (ptr == NULL) {
		return NULL;
	}
	return memset(ptr, 0, size);
}

void m_free(void* memory, size_t size) {
	app_get_stats()->memory -= size;
	free(memory);
}