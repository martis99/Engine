#include "pch.h"

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

	vec4 l1 = { l.a11, l.a12, l.a13, l.a14 };
	vec4 l2 = { l.a21, l.a22, l.a23, l.a24 };
	vec4 l3 = { l.a31, l.a32, l.a33, l.a34 };
	vec4 l4 = { l.a41, l.a42, l.a43, l.a44 };

	vec4 r1 = { r.a11, r.a21, r.a31, r.a41 };
	vec4 r2 = { r.a12, r.a22, r.a32, r.a42 };
	vec4 r3 = { r.a13, r.a23, r.a33, r.a43 };
	vec4 r4 = { r.a14, r.a24, r.a34, r.a44 };

	mat.a11 = vec4_dot(l1, r1);
	mat.a12 = vec4_dot(l1, r2);
	mat.a13 = vec4_dot(l1, r3);
	mat.a14 = vec4_dot(l1, r4);

	mat.a21 = vec4_dot(l2, r1);
	mat.a22 = vec4_dot(l2, r2);
	mat.a23 = vec4_dot(l2, r3);
	mat.a24 = vec4_dot(l2, r4);

	mat.a31 = vec4_dot(l3, r1);
	mat.a32 = vec4_dot(l3, r2);
	mat.a33 = vec4_dot(l3, r3);
	mat.a34 = vec4_dot(l3, r4);

	mat.a41 = vec4_dot(l4, r1);
	mat.a42 = vec4_dot(l4, r2);
	mat.a43 = vec4_dot(l4, r3);
	mat.a44 = vec4_dot(l4, r4);

	return mat;
}

vec3 mat4_mul_vec3(mat4 m, vec3 v) {
	vec4 vec = mat4_mul_vec4(m, (vec4) { v.x, v.y, v.z, 1.0f });
	return (vec3) { vec.x, vec.y, vec.z };
}

vec4 mat4_mul_vec4(mat4 m, vec4 v) {
	vec4 vec;

	vec4 m1 = { m.a11, m.a12, m.a13, m.a14 };
	vec4 m2 = { m.a21, m.a22, m.a23, m.a24 };
	vec4 m3 = { m.a31, m.a32, m.a33, m.a34 };
	vec4 m4 = { m.a41, m.a42, m.a43, m.a44 };

	vec.x = vec4_dot(m1, v);
	vec.y = vec4_dot(m2, v);
	vec.z = vec4_dot(m3, v);
	vec.w = vec4_dot(m4, v);

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