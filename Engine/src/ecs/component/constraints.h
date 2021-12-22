#pragma once
#include "ecs/ecs_types.h"

Constraint constraint_create(Entity entity, float pos, int distance);

Constraints constraints_create(vec3 size);