#pragma once
#include "ecs_types.h"

Ecs* ecs_create(Ecs* ecs, size_t component_count, ...);
void ecs_delete(Ecs* ecs);

Entity ecs_entity(Ecs* ecs);
void* ecs_get(Ecs* ecs, Id entity_id, Id component_id);
void ecs_add(Ecs* ecs, Id entity_id, Id component_id, void* data);
void ecs_remove(Ecs* ecs, Id entity_id, Id component_id);
Id ecs_has(Ecs* ecs, Id entity_id, Id component_id);
void ecs_kill(Ecs* ecs, Id entity_id);

QueryResult* ecs_query(Ecs* ecs, size_t component_count, ...);
