#include "quaternion.h"
#include "mat4.h"
#include "vec3.h"

#include <math.h>
#include <stdlib.h>

quaternion quaternion_zero()
{
	return (quaternion){ 0 };
}

quaternion euler_to_quaternion(vec3 euler)
{
	vec3 c = vec3_cos(vec3_mulf(euler, 0.5f));
	vec3 s = vec3_sin(vec3_mulf(euler, 0.5f));

	return (quaternion){
		.w = c.x * c.y * c.z + s.x * s.y * s.z,
		.x = s.x * c.y * c.z - c.x * s.y * s.z,
		.y = c.x * s.y * c.z + s.x * c.y * s.z,
		.z = c.x * c.y * s.z - s.x * s.y * c.z,
	};
}

vec3 quaternion_to_euler(quaternion q)
{
	vec3 euler = { 0 };

	float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);

	euler.x = atan2f(sinr_cosp, cosr_cosp);

	float sinp = 2 * (q.w * q.y - q.z * q.x);
	if (abs((int)sinp) >= 1) {
		euler.y = copysignf(1.570796f, sinp);
	} else {
		euler.y = asinf(sinp);
	}

	float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	euler.z		= atan2f(siny_cosp, cosy_cosp);

	return euler;
}

mat4 quaternion_to_mat4(quaternion value)
{
	float qxx = value.x * value.x;
	float qyy = value.y * value.y;
	float qzz = value.z * value.z;
	float qxz = value.x * value.z;
	float qxy = value.x * value.y;
	float qyz = value.y * value.z;
	float qwx = value.w * value.x;
	float qwy = value.w * value.y;
	float qwz = value.w * value.z;

	return (mat4){
		.a11 = 1.0f - 2.0f * (qyy + qzz),
		.a12 = 2.0f * (qxy + qwz),
		.a13 = 2.0f * (qxy - qwy),

		.a21 = 2.0f * (qxy - qwz),
		.a22 = 1.0f - 2.0f * (qxx + qzz),
		.a23 = 2.0f * (qyz + qwx),

		.a31 = 2.0f * (qxz + qwy),
		.a32 = 2.0f * (qyz - qwx),
		.a33 = 1.0f - 2.0f * (qxx + qyy),

		.a44 = 1.0f,
	};
}
