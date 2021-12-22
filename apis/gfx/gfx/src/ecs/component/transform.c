#include "pch.h"
#include "transform.h"

Transform transform_create(vec3 position, vec3 rotation, vec3 scale) {
	return (Transform) { position, rotation, scale, (vec3) { 0.0f, 0.0f, 0.0f } };
}

mat4 transform_to_mat4(Transform* transform) {
	return  mat4_mul(mat4_scale(transform->scale), mat4_mul(quaternion_to_mat4(euler_to_quaternion(transform->rotation)), mat4_translation(vec3_mul(transform->position, (vec3) { 1, 1, -1 }))));
}

vec3 transform_vec3(Transform* transform, vec3 v) {
	return mat4_mul_vec3(transform_to_mat4(transform), v);
}