#include "pch.h"
#include "transform.h"

Transform transform_create(vec3 position, vec3 rotation, vec3 scale) {
	return (Transform) { position, rotation, scale, (vec3) { 0.0f, 0.0f, 0.0f } };
}

Transform transform_create_2d(vec2i position, float rotation, vec2i scale) {
	return (Transform) { vec2i_to_vec3(position, 0.0f), (vec3) { 0.0f, rotation, 0.0f }, vec2i_to_vec3(scale, 1.0f), (vec3) { 0.0f, 0.0f, 0.0f } };
}

mat4 transform_to_mat4(Transform* transform) {
	vec3 position = transform->position;
	position.z = -position.z;
	return  mat4_mul(mat4_scale(transform->scale), mat4_mul(quaternion_to_mat4(euler_to_quaternion(transform->rotation)), mat4_translation(position)));
}

vec3 transform_vec3(Transform* transform, vec3 v) {
	return mat4_mul_vec3(mat4_invert(transform_to_mat4(transform)), v);
}