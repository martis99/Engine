#include "mat4.h"
#include <xmmintrin.h>

#include "vec3.h"

#include <math.h>

float deg2rad(float deg) {
	return deg * 3.14159265f / 180.0f;
}

mat4 mat4_identity() {
	mat4 mat = { 0 };
	mat.a11 = 1.0f;
	mat.a22 = 1.0f;
	mat.a33 = 1.0f;
	mat.a44 = 1.0f;
	return mat;
}

mat4 mat4_scalef(float value) {
	mat4 mat = { 0 };
	mat.a11 = value;
	mat.a22 = value;
	mat.a33 = value;
	mat.a44 = 1.0f;
	return mat;
}

mat4 mat4_translation(vec3 value) {
	mat4 mat = mat4_identity();
	mat.a41 = value.x;
	mat.a42 = value.y;
	mat.a43 = value.z;
	mat.a44 = 1.0f;
	return mat;
}

mat4 mat4_rotation_x(float value) {
	mat4 mat = mat4_identity();

	mat.a22 = cosf(value);
	mat.a23 = sinf(value);
	mat.a32 = -sinf(value);
	mat.a33 = cosf(value);

	return mat;
}

mat4 mat4_rotation_y(float value) {
	mat4 mat = mat4_identity();

	mat.a11 = cosf(value);
	mat.a13 = -sinf(value);
	mat.a31 = sinf(value);
	mat.a33 = cosf(value);

	return mat;
}

mat4 mat4_rotation_z(float value) {
	mat4 mat = mat4_identity();

	mat.a11 = cosf(value);
	mat.a12 = sinf(value);
	mat.a21 = -sinf(value);
	mat.a22 = cosf(value);

	return mat;
}

mat4 mat4_scale(vec3 value) {
	mat4 mat = { 0 };
	mat.a11 = value.x;
	mat.a22 = value.y;
	mat.a33 = value.z;
	mat.a44 = 1.0f;
	return mat;
}

mat4 mat4_invert(mat4 m) {
	mat4 mat = { 0 };

	mat.a11 = m.a11;
	mat.a12 = m.a21;
	mat.a13 = m.a31;
	mat.a14 = m.a41;

	mat.a21 = m.a12;
	mat.a22 = m.a22;
	mat.a23 = m.a32;
	mat.a24 = m.a42;

	mat.a31 = m.a13;
	mat.a32 = m.a23;
	mat.a33 = m.a33;
	mat.a34 = m.a43;

	mat.a41 = m.a14;
	mat.a42 = m.a24;
	mat.a43 = m.a34;
	mat.a44 = m.a44;

	return mat;
}

mat4 mat4_mul(mat4 l, mat4 r) {
	mat4 mat = { 0 };

	float* A = (float*)&l;
	float* C = (float*)&mat;

	__m128 row1 = _mm_load_ps(&r.a11);
	__m128 row2 = _mm_load_ps(&r.a21);
	__m128 row3 = _mm_load_ps(&r.a31);
	__m128 row4 = _mm_load_ps(&r.a41);
	for (int i = 0; i < 4; i++) {
		__m128 brod1 = _mm_set1_ps(A[4 * i + 0]);
		__m128 brod2 = _mm_set1_ps(A[4 * i + 1]);
		__m128 brod3 = _mm_set1_ps(A[4 * i + 2]);
		__m128 brod4 = _mm_set1_ps(A[4 * i + 3]);
		__m128 row = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(brod1, row1),
				_mm_mul_ps(brod2, row2)),
			_mm_add_ps(
				_mm_mul_ps(brod3, row3),
				_mm_mul_ps(brod4, row4)));
		_mm_store_ps(&C[4 * i], row);
	}
	return mat;
}

vec3 mat4_mul_vec3(mat4 m, vec3 v) {
	vec4 vec = mat4_mul_vec4(m, (vec4) { v.x, v.y, v.z, 1.0f });
	return (vec3) { vec.x, vec.y, vec.z };
}

vec4 mat4_mul_vec4(mat4 m, vec4 v) {
	__m128 x = _mm_set1_ps(v.x);
	__m128 y = _mm_set1_ps(v.y);
	__m128 z = _mm_set1_ps(v.z);
	__m128 w = _mm_set1_ps(v.w);

	__m128 row1 = _mm_load_ps(&m.a11);
	__m128 row2 = _mm_load_ps(&m.a21);
	__m128 row3 = _mm_load_ps(&m.a31);
	__m128 row4 = _mm_load_ps(&m.a41);

	__m128 p1 = _mm_mul_ps(x, row1);
	__m128 p2 = _mm_mul_ps(y, row2);
	__m128 p3 = _mm_mul_ps(z, row3);
	__m128 p4 = _mm_mul_ps(w, row4);

	vec4 vec;
	_mm_store_ps((float*)&vec, _mm_add_ps(_mm_add_ps(p1, p2), _mm_add_ps(p3, p4)));
	return vec;
}

mat4 mat4_perspective(float fovy, float aspect, float zNear, float zFar) {
	mat4 mat = mat4_identity();

	float tanHalfFovy = tanf(fovy / 2.0f);

	mat.a11 = 1.0f / (aspect * tanHalfFovy);
	mat.a22 = 1 / tanHalfFovy;
	mat.a33 = -(zFar + zNear) / (zFar - zNear);
	mat.a34 = -1.0f;
	mat.a42 = -(2.0f * zFar * zNear) / (zFar - zNear);

	return mat;
}

mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
	mat4 mat = mat4_identity();
	mat.a11 = 2.0f / (right - left);
	mat.a22 = 2.0f / (top - bottom);
	mat.a33 = -2.0f / (far - near);
	mat.a41 = -(right + left) / (right - left);
	mat.a42 = -(top + bottom) / (top - bottom);
	mat.a43 = -(far + near) / (far - near);
	return mat;
}

void swap(float* l, float* r) {
	float t = *l;
	*l = *r;
	*r = t;
}

mat4 lh_to_rh(mat4 value) {
	mat4 mat = value;

	swap(&mat.a12, &mat.a13);
	swap(&mat.a22, &mat.a23);
	swap(&mat.a32, &mat.a33);
	swap(&mat.a42, &mat.a43);

	swap(&mat.a21, &mat.a31);
	swap(&mat.a22, &mat.a32);
	swap(&mat.a23, &mat.a33);
	swap(&mat.a24, &mat.a34);

	return mat;
}

mat4 mat4_look_at_lh(vec3 eye, vec3 center, vec3 up) {
	vec3 f = vec3_normalize(vec3_sub(center, eye));
	vec3 s = vec3_normalize(vec3_cross(up, f));
	vec3 u = vec3_cross(f, s);

	mat4 mat = mat4_identity();
	mat.a11 = s.x;
	mat.a21 = s.y;
	mat.a31 = s.z;
	mat.a12 = u.x;
	mat.a22 = u.y;
	mat.a32 = u.z;
	mat.a13 = f.x;
	mat.a23 = f.y;
	mat.a33 = f.z;
	mat.a41 = -vec3_dot(s, eye);
	mat.a42 = -vec3_dot(u, eye);
	mat.a43 = -vec3_dot(f, eye);

	return mat;
}

mat4 mat4_look_at_rh(vec3 eye, vec3 center, vec3 up) {
	vec3 f = vec3_normalize(vec3_sub(center, eye));
	vec3 s = vec3_normalize(vec3_cross(f, up));
	vec3 u = vec3_cross(s, f);

	mat4 mat = mat4_identity();
	mat.a11 = s.x;
	mat.a21 = s.y;
	mat.a31 = s.z;
	mat.a12 = u.x;
	mat.a22 = u.y;
	mat.a32 = u.z;
	mat.a13 = -f.x;
	mat.a23 = -f.y;
	mat.a33 = -f.z;
	mat.a41 = -vec3_dot(s, eye);
	mat.a42 = -vec3_dot(u, eye);
	mat.a43 = vec3_dot(f, eye);

	return mat;
}