#include "quaternion.h"
#include "vec3.h"
#include "mat4.h"

#include <math.h>

quaternion quaternion_zero() {
	quaternion q = { 0 };
	return q;
}

quaternion euler_to_quaternion(vec3 euler) {
	vec3 c = vec3_cos(vec3_mulf(euler, 0.5f));
	vec3 s = vec3_sin(vec3_mulf(euler, 0.5f));

	quaternion q;
	q.w = c.x * c.y * c.z + s.x * s.y * s.z;
	q.x = s.x * c.y * c.z - c.x * s.y * s.z;
	q.y = c.x * s.y * c.z + s.x * c.y * s.z;
	q.z = c.x * c.y * s.z - s.x * s.y * c.z;

	return q;
}

vec3 quaternion_to_euler(quaternion q) {
	vec3 euler;

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
	euler.z = atan2f(siny_cosp, cosy_cosp);

	return euler;
}

mat4 quaternion_to_mat4(quaternion value) {
	mat4 mat = mat4_identity();

	float qxx = value.x * value.x;
	float qyy = value.y * value.y;
	float qzz = value.z * value.z;
	float qxz = value.x * value.z;
	float qxy = value.x * value.y;
	float qyz = value.y * value.z;
	float qwx = value.w * value.x;
	float qwy = value.w * value.y;
	float qwz = value.w * value.z;

	mat.a11 = 1.0f - 2.0f * (qyy + qzz);
	mat.a12 = 2.0f * (qxy + qwz);
	mat.a13 = 2.0f * (qxy - qwy);

	mat.a21 = 2.0f * (qxy - qwz);
	mat.a22 = 1.0f - 2.0f * (qxx + qzz);
	mat.a23 = 2.0f * (qyz + qwx);

	mat.a31 = 2.0f * (qxz + qwy);
	mat.a32 = 2.0f * (qyz - qwx);
	mat.a33 = 1.0f - 2.0f * (qxx + qyy);

	return mat;
}