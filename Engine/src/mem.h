#pragma once

void* m_realloc(void* memory, size_t new_size, size_t old_size);
void* m_calloc(size_t count, size_t size);
void* m_malloc(size_t size);
void m_free(void* memory, size_t size);