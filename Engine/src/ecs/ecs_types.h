#pragma once
#include "eng_common_types.h"
#include "utils/data_types.h"
#include "math/math_types.h"

#define ENTITY_FLAG_ALIVE 1

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

typedef struct Constraint {
	bool enabled;
	Entity entity;
	float pos;
	int distance;
} Constraint;

typedef struct Constraints {
	vec3 resolved;
	vec3 size;
	Constraint l, r, u, d, f, b;
} Constraints;

typedef struct Transform {
	vec3 position;
	vec3 rotation;
	vec3 scale;
	vec3 scale_pref;
} Transform;
