#include "pch.h"
#include "convert.h"

vec2 vec2i_to_vec2(vec2i value) {
	return (vec2) { (float)value.x, (float)value.y };
}

vec3 vec2i_to_vec3(vec2i value, float z) {
	return (vec3) { (float)value.x, (float)value.y, z };
}

vec3 vec2_to_vec3(vec2 value, float z) {
	return (vec3) { value.x, value.y, z };
}

vec2 vec3_to_vec2(vec3 value) {
	return (vec2) { value.x, value.y };
}

vec4 vec3_to_vec4(vec3 value, float w) {
	return (vec4) { value.x, value.y, value.z, w };
}