#include "ecs.h"

#include "mem.h"

#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 2

Ecs *ecs_create(Ecs *ecs, size_t n, ...)
{
	size_t i;
	va_list ap;
	size_t sizes[32]   = { 0 };
	size_t offsets[32] = { 0 };
	size_t size	   = 0;

	va_start(ap, n);
	for (i = 0; i < n; ++i) {
		sizes[i]   = va_arg(ap, size_t);
		offsets[i] = size;
		size += sizes[i];
	}
	va_end(ap);

	ecs->entity_pool = stack_create(sizeof(Id));

	ecs->component_store.type_count	       = n;
	ecs->component_store.cap	       = INITIAL_CAPACITY;
	ecs->component_store.data	       = m_malloc(INITIAL_CAPACITY * size);
	ecs->component_store.data_size_array   = m_malloc(n * sizeof(size_t));
	ecs->component_store.data_offset_array = m_malloc(n * sizeof(size_t));
	ecs->component_store.size	       = size;
	if (ecs->component_store.data_size_array != NULL) {
		memcpy(ecs->component_store.data_size_array, sizes, n * sizeof(size_t));
	}
	if (ecs->component_store.data_offset_array != NULL) {
		memcpy(ecs->component_store.data_offset_array, offsets, n * sizeof(size_t));
	}

	ecs->entity_store.count	     = 0;
	ecs->entity_store.cap	     = INITIAL_CAPACITY;
	ecs->entity_store.mask_array = m_malloc(INITIAL_CAPACITY * sizeof(Id));
	ecs->entity_store.flag_array = m_malloc(INITIAL_CAPACITY * sizeof(Id));

	ecs->query_result.cap  = INITIAL_CAPACITY;
	ecs->query_result.list = m_malloc(INITIAL_CAPACITY * sizeof(Id));

	return ecs;
}

void ecs_delete(Ecs *ecs)
{
	m_free(ecs->component_store.data, ecs->component_store.cap * ecs->component_store.size);
	m_free(ecs->component_store.data_size_array, ecs->component_store.type_count * sizeof(size_t));
	m_free(ecs->component_store.data_offset_array, ecs->component_store.type_count * sizeof(size_t));
	m_free(ecs->entity_store.mask_array, ecs->entity_store.cap * sizeof(Id));
	m_free(ecs->entity_store.flag_array, ecs->entity_store.cap * sizeof(Id));
	m_free(ecs->query_result.list, ecs->query_result.cap * sizeof(Id));

	stack_delete(ecs->entity_pool);
}

Entity ecs_entity(Ecs *ecs)
{
	Entity entity = { 0 };
	Id id;
	if (ecs->entity_pool->count > 0) {
		id = *(Id *)stack_pop(ecs->entity_pool);
	} else {
		id = (Id)ecs->entity_store.count++;
		if (ecs->entity_store.cap == id) {
			Id *new_flag_array	  = m_realloc(ecs->entity_store.flag_array, ecs->entity_store.cap * 2 * sizeof(Id), ecs->entity_store.cap * sizeof(Id));
			Id *new_mask_array	  = m_realloc(ecs->entity_store.mask_array, ecs->entity_store.cap * 2 * sizeof(Id), ecs->entity_store.cap * sizeof(Id));
			void *new_data		  = m_realloc(ecs->component_store.data, ecs->component_store.cap * 2 * ecs->component_store.size,
							      ecs->component_store.cap * ecs->component_store.size);
			Id *new_query_result_list = m_realloc(ecs->query_result.list, ecs->query_result.cap * 2 * sizeof(Id), ecs->query_result.cap * sizeof(Id));
			if (NULL == new_flag_array || NULL == new_mask_array || NULL == new_data || NULL == new_query_result_list) {
				printf("Realloc fail %s:%d\n", __FILE__, __LINE__);
				exit(1);
			} else {
				ecs->entity_store.flag_array = new_flag_array;
				ecs->entity_store.mask_array = new_mask_array;
				ecs->query_result.list	     = new_query_result_list;
				ecs->entity_store.cap *= 2;

				ecs->query_result.cap *= 2;

				ecs->component_store.data = new_data;
				ecs->component_store.cap *= 2;
			}
		}
	}

	ecs->entity_store.mask_array[id] = 0;
	ecs->entity_store.flag_array[id] = ENTITY_FLAG_ALIVE;

	entity.id = id;
	return entity;
}

void *ecs_get(Ecs *ecs, Id entity_id, Id component_id)
{
	return (byte *)ecs->component_store.data + (entity_id * ecs->component_store.size + ecs->component_store.data_offset_array[component_id]);
}

void ecs_add(Ecs *ecs, Id entity_id, Id component_id, void *data)
{
	size_t size = ecs->component_store.data_size_array[component_id];
	void *ptr   = ecs_get(ecs, entity_id, component_id);
	ecs->entity_store.mask_array[entity_id] |= (1 << component_id);
	memcpy(ptr, data, size);
}

void ecs_remove(Ecs *ecs, Id entity_id, Id component_id)
{
	ecs->entity_store.mask_array[entity_id] &= ~(1 << component_id);
}

Id ecs_has(Ecs *ecs, Id entity_id, Id component_id)
{
	return 0 != (ecs->entity_store.mask_array[entity_id] & (1 << component_id));
}

void ecs_kill(Ecs *ecs, Id entity_id)
{
	if (0 != (ecs->entity_store.flag_array[entity_id] & ENTITY_FLAG_ALIVE)) {
		ecs->entity_store.flag_array[entity_id] &= ~ENTITY_FLAG_ALIVE;
		ecs->entity_store.mask_array[entity_id] = 0;
		stack_push(ecs->entity_pool, &entity_id);
	}
}

QueryResult *ecs_query(Ecs *ecs, size_t n, ...)
{
	va_list ap;
	Id i, mask = 0;

	ecs->query_result.count = 0;

	va_start(ap, n);
	for (i = 0; i < n; ++i) {
		mask |= (1 << va_arg(ap, Id));
	}
	va_end(ap);

	for (i = 0; i < ecs->entity_store.count; ++i) {
		if (0 != (ecs->entity_store.flag_array[i] & ENTITY_FLAG_ALIVE) && mask == (ecs->entity_store.mask_array[i] & mask)) {
			ecs->query_result.list[ecs->query_result.count++] = i;
		}
	}
	return &ecs->query_result;
}
