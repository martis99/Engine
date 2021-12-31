#include "vec3.h"

static const vec3 right = { 1.0f, 0.0f, 0.0f };
static const vec3 up = { 0.0f, 1.0f, 0.0f };
static const vec3 front = { 0.0f, 0.0f, 1.0f };

#include <math.h>

vec3 vec3_mulf(vec3 value, float scalar) {
	vec3 vec = { value.x * scalar, value.y * scalar, value.z * scalar };
	return vec;
}

vec3 vec3_sub(vec3 l, vec3 r) {
	vec3 vec = { l.x - r.x, l.y - r.y, l.z - r.z };
	return vec;
}

vec3 vec3_add(vec3 l, vec3 r) {
	vec3 vec = { l.x + r.x, l.y + r.y, l.z + r.z };
	return vec;
}

vec3 vec3_mul(vec3 l, vec3 r) {
	vec3 vec = { l.x * r.x, l.y * r.y, l.z * r.z };
	return vec;
}

float vec3_length(vec3 value) {
	return sqrtf(vec3_dot(value, value));
}

vec3 vec3_normalize(vec3 value) {
	float length = vec3_length(value);
	vec3 vec = { value.x / length, value.y / length, value.z / length };
	return vec;
}

float vec3_dot(vec3 l, vec3 r) {
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

vec3 vec3_cross(vec3 l, vec3 r) {
	vec3 vec = {
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	};
	return vec;
}

vec3 vec3_right() {
	return right;
}

vec3 vec3_up() {
	return up;
}

vec3 vec3_front() {
	return front;
}

vec3 vec3_sin(vec3 value) {
	return (vec3) { sinf(value.x), sinf(value.y), sinf(value.z) };
}

vec3 vec3_cos(vec3 value) {
	return (vec3) { cosf(value.x), cosf(value.y), cosf(value.z) };
}