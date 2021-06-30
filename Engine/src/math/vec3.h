#pragma once

typedef struct vec3 {
	float x, y, z;
} vec3;

vec3 vec3_mulf(vec3 value, float scalar);
vec3 vec3_sub(vec3 l, vec3 r);
vec3 vec3_add(vec3 l, vec3 r);
vec3 vec3_mul(vec3 l, vec3 r);

float vec3_length(vec3 value);

vec3 vec3_normalize(vec3 value);

float vec3_dot(vec3 l, vec3 r);
vec3 vec3_cross(vec3 l, vec3 r);

vec3 vec3_right();
vec3 vec3_up();
vec3 vec3_front();

vec3 vec3_sin(vec3 value);
vec3 vec3_cos(vec3 value);