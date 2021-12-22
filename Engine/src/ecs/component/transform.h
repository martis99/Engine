#pragma once
#include "ecs/ecs_types.h"

Transform transform_create(vec3 position, vec3 rotation, vec3 scale);
mat4 transform_to_mat4(Transform* transform);
vec3 transform_vec3(Transform* transform, vec3 v);