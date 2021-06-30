#pragma once

typedef struct Transform {
	vec3 position;
	vec3 rotation;
	vec3 scale;
	vec3 scale_pref;
} Transform;

Transform transform_create(vec3 position, vec3 rotation, vec3 scale);
Transform transform_create_2d(vec2i position, float rotation, vec2i scale);
mat4 transform_to_mat4(Transform* transform);
vec3 transform_vec3(Transform* transform, vec3 v);