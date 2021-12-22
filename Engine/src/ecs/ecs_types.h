#pragma once
#include "eng_common_types.h"
#include "utils/data_types.h"

#define ENTITY_FLAG_ALIVE 1
#define C_TRANSFORM 0
#define C_MESH 1
#define C_SPRITE 2
#define C_TEXT 3
#define C_CONSTRAINTS 4
#define C_INSTANCE 5
#define C_MODEL 6

typedef uint Id;

typedef struct Entity {
	Id id;
} Entity;

typedef struct QueryResult {
	size_t count;
	size_t cap;
	Id* list;
} QueryResult;

typedef struct ComponentStore {
	size_t type_count;
	size_t cap;
	size_t size;
	size_t* data_size_array;
	size_t* data_offset_array;
	void* data;
} ComponentStore;

typedef struct EntityStore {
	Id* mask_array;
	Id* flag_array;
	size_t count;
	size_t cap;
} EntityStore;

typedef struct Ecs {
	ComponentStore component_store;
	EntityStore entity_store;
	QueryResult query_result;
	Stack* entity_pool;
} Ecs;