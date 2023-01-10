#include "vec4.h"

float vec4_dot(vec4 l, vec4 r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}
