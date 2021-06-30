#pragma once

typedef struct vec4 {
	float x, y, z, w;
} vec4;

float vec4_dot(vec4 l, vec4 r);