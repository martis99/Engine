#include "vec3.h"

#include <math.h>

static const vec3 right = { 1.0f, 0.0f, 0.0f };
static const vec3 up	= { 0.0f, 1.0f, 0.0f };

#ifdef LHC
static const vec3 front = { 0.0f, 0.0f, 1.0f };
#else
static const vec3 front = { 0.0f, 0.0f, -1.0f };
#endif

vec3 vec3_mulf(vec3 value, float scalar)
{
	return (vec3){ value.x * scalar, value.y * scalar, value.z * scalar };
}

vec3 vec3_sub(vec3 l, vec3 r)
{
	return (vec3){ l.x - r.x, l.y - r.y, l.z - r.z };
}

vec3 vec3_add(vec3 l, vec3 r)
{
	return (vec3){ l.x + r.x, l.y + r.y, l.z + r.z };
}

vec3 vec3_mul(vec3 l, vec3 r)
{
	return (vec3){ l.x * r.x, l.y * r.y, l.z * r.z };
}

float vec3_length(vec3 value)
{
	return sqrtf(vec3_dot(value, value));
}

vec3 vec3_normalize(vec3 value)
{
	float length = vec3_length(value);
	return (vec3){ value.x / length, value.y / length, value.z / length };
}

float vec3_dot(vec3 l, vec3 r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

vec3 vec3_cross(vec3 l, vec3 r)
{
	// clang-format off
	return (vec3){
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	};
	// clang-format on
}

vec3 vec3_right()
{
	return right;
}

vec3 vec3_up()
{
	return up;
}

vec3 vec3_front()
{
	return front;
}

vec3 vec3_sin(vec3 value)
{
	return (vec3){ sinf(value.x), sinf(value.y), sinf(value.z) };
}

vec3 vec3_cos(vec3 value)
{
	return (vec3){ cosf(value.x), cosf(value.y), cosf(value.z) };
}
