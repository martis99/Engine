#include "mat4.h"

#include "vec3.h"

#include <math.h>
#include <xmmintrin.h>

#ifdef LHC
static const int hc = -1;
#else
static const int hc = 1;
#endif

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
	mat4 mat = mat4_identity();
	mat.a11 = value;
	mat.a22 = value;
	mat.a33 = value;
	return mat;
}

mat4 mat4_translation(vec3 value) {
	mat4 mat = mat4_identity();
	mat.a14 = value.x;
	mat.a24 = value.y;
	mat.a34 = value.z;
	return mat;
}

mat4 mat4_rotation(vec3 value) {
	return mat4_mul(mat4_rotation_x(value.x), mat4_mul(mat4_rotation_y(value.y), mat4_rotation_z(value.z)));
}

mat4 mat4_rotation_x(float value) {
	mat4 mat = mat4_identity();
	mat.a22 = cosf(value);
	mat.a23 = hc * -sinf(value);
	mat.a32 = hc * sinf(value);
	mat.a33 = cosf(value);
	return mat;
}

mat4 mat4_rotation_y(float value) {
	mat4 mat = mat4_identity();
	mat.a11 = cosf(value);
	mat.a13 = hc * sinf(value);
	mat.a31 = hc * -sinf(value);
	mat.a33 = cosf(value);
	return mat;
}

mat4 mat4_rotation_z(float value) {
	mat4 mat = mat4_identity();
	mat.a11 = cosf(value);
	mat.a12 = -sinf(value);
	mat.a21 = sinf(value);
	mat.a22 = cosf(value);
	return mat;
}

mat4 mat4_scale(vec3 value) {
	mat4 mat = mat4_identity();
	mat.a11 = value.x;
	mat.a22 = value.y;
	mat.a33 = value.z;
	return mat;
}

mat4 mat4_transpose(mat4 m) {
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

mat4 mat4_invert(mat4 m) {
	mat4 mat;

	mat.a[0] = m.a[5]  * m.a[10] * m.a[15] -
		m.a[5]  * m.a[11] * m.a[14] -
		m.a[9]  * m.a[6]  * m.a[15] +
		m.a[9]  * m.a[7]  * m.a[14] +
		m.a[13] * m.a[6]  * m.a[11] -
		m.a[13] * m.a[7]  * m.a[10];

	mat.a[4] =
		-m.a[4]  * m.a[10] * m.a[15] +
		 m.a[4]  * m.a[11] * m.a[14] +
		 m.a[8]  * m.a[6]  * m.a[15] -
		 m.a[8]  * m.a[7]  * m.a[14] -
		 m.a[12] * m.a[6]  * m.a[11] +
		 m.a[12] * m.a[7]  * m.a[10];

	mat.a[8] =
		m.a[4]  * m.a[9]  * m.a[15] -
		m.a[4]  * m.a[11] * m.a[13] -
		m.a[8]  * m.a[5]  * m.a[15] +
		m.a[8]  * m.a[7]  * m.a[13] +
		m.a[12] * m.a[5]  * m.a[11] -
		m.a[12] * m.a[7]  * m.a[9];

	mat.a[12] =
		-m.a[4]  * m.a[9]  * m.a[14] +
		 m.a[4]  * m.a[10] * m.a[13] +
		 m.a[8]  * m.a[5]  * m.a[14] -
		 m.a[8]  * m.a[6]  * m.a[13] -
		 m.a[12] * m.a[5]  * m.a[10] +
		 m.a[12] * m.a[6]  * m.a[9];

	mat.a[1] =
		-m.a[1]  * m.a[10] * m.a[15] +
		 m.a[1]  * m.a[11] * m.a[14] +
		 m.a[9]  * m.a[2]  * m.a[15] -
		 m.a[9]  * m.a[3]  * m.a[14] -
		 m.a[13] * m.a[2]  * m.a[11] +
		 m.a[13] * m.a[3]  * m.a[10];

	mat.a[5] =
		m.a[0]  * m.a[10] * m.a[15] -
		m.a[0]  * m.a[11] * m.a[14] -
		m.a[8]  * m.a[2]  * m.a[15] +
		m.a[8]  * m.a[3]  * m.a[14] +
		m.a[12] * m.a[2]  * m.a[11] -
		m.a[12] * m.a[3]  * m.a[10];

	mat.a[9] =
		-m.a[0]  * m.a[9]  * m.a[15] +
		 m.a[0]  * m.a[11] * m.a[13] +
		 m.a[8]  * m.a[1]  * m.a[15] -
		 m.a[8]  * m.a[3]  * m.a[13] -
		 m.a[12] * m.a[1]  * m.a[11] +
		 m.a[12] * m.a[3]  * m.a[9];

	mat.a[13] =
		m.a[0]  * m.a[9]  * m.a[14] -
		m.a[0]  * m.a[10] * m.a[13] -
		m.a[8]  * m.a[1]  * m.a[14] +
		m.a[8]  * m.a[2]  * m.a[13] +
		m.a[12] * m.a[1]  * m.a[10] -
		m.a[12] * m.a[2]  * m.a[9];

	mat.a[2] =
		m.a[1]  * m.a[6] * m.a[15] -
		m.a[1]  * m.a[7] * m.a[14] -
		m.a[5]  * m.a[2] * m.a[15] +
		m.a[5]  * m.a[3] * m.a[14] +
		m.a[13] * m.a[2] * m.a[7] -
		m.a[13] * m.a[3] * m.a[6];

	mat.a[6] =
		-m.a[0]  * m.a[6] * m.a[15] +
		 m.a[0]  * m.a[7] * m.a[14] +
		 m.a[4]  * m.a[2] * m.a[15] -
		 m.a[4]  * m.a[3] * m.a[14] -
		 m.a[12] * m.a[2] * m.a[7] +
		 m.a[12] * m.a[3] * m.a[6];

	mat.a[10] =
		m.a[0]  * m.a[5] * m.a[15] -
		m.a[0]  * m.a[7] * m.a[13] -
		m.a[4]  * m.a[1] * m.a[15] +
		m.a[4]  * m.a[3] * m.a[13] +
		m.a[12] * m.a[1] * m.a[7] -
		m.a[12] * m.a[3] * m.a[5];

	mat.a[14] =
		-m.a[0]  * m.a[5] * m.a[14] +
		 m.a[0]  * m.a[6] * m.a[13] +
		 m.a[4]  * m.a[1] * m.a[14] -
		 m.a[4]  * m.a[2] * m.a[13] -
		 m.a[12] * m.a[1] * m.a[6] +
		 m.a[12] * m.a[2] * m.a[5];

	mat.a[3] =
		-m.a[1] * m.a[6] * m.a[11] +
		 m.a[1] * m.a[7] * m.a[10] +
		 m.a[5] * m.a[2] * m.a[11] -
		 m.a[5] * m.a[3] * m.a[10] -
		 m.a[9] * m.a[2] * m.a[7] +
		 m.a[9] * m.a[3] * m.a[6];

	mat.a[7] =
		m.a[0] * m.a[6] * m.a[11] -
		m.a[0] * m.a[7] * m.a[10] -
		m.a[4] * m.a[2] * m.a[11] +
		m.a[4] * m.a[3] * m.a[10] +
		m.a[8] * m.a[2] * m.a[7] -
		m.a[8] * m.a[3] * m.a[6];

	mat.a[11] =
		-m.a[0] * m.a[5] * m.a[11] +
		 m.a[0] * m.a[7] * m.a[9] +
		 m.a[4] * m.a[1] * m.a[11] -
		 m.a[4] * m.a[3] * m.a[9] -
		 m.a[8] * m.a[1] * m.a[7] +
		 m.a[8] * m.a[3] * m.a[5];

	mat.a[15] =
		m.a[0] * m.a[5] * m.a[10] -
		m.a[0] * m.a[6] * m.a[9] -
		m.a[4] * m.a[1] * m.a[10] +
		m.a[4] * m.a[2] * m.a[9] +
		m.a[8] * m.a[1] * m.a[6] -
		m.a[8] * m.a[2] * m.a[5];

	float det = m.a[0] * mat.a[0] + m.a[1] * mat.a[4] + m.a[2] * mat.a[8] + m.a[3] * mat.a[12];

	if (det == 0)
		return mat4_identity();

	det = 1.0f / det;

	for (int i = 0; i < 16; i++)
		mat.a[i] = mat.a[i] * det;

	return mat;
}

mat4 mat4_mul(mat4 l, mat4 r) {
	mat4 mat = { 0 };
	__m128 row1 = _mm_load_ps(&r.a11);
	__m128 row2 = _mm_load_ps(&r.a21);
	__m128 row3 = _mm_load_ps(&r.a31);
	__m128 row4 = _mm_load_ps(&r.a41);
	for (int i = 0; i < 4; i++) {
		__m128 brod1 = _mm_set1_ps(l.a[4 * i + 0]);
		__m128 brod2 = _mm_set1_ps(l.a[4 * i + 1]);
		__m128 brod3 = _mm_set1_ps(l.a[4 * i + 2]);
		__m128 brod4 = _mm_set1_ps(l.a[4 * i + 3]);
		__m128 row = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(brod1, row1),
				_mm_mul_ps(brod2, row2)),
			_mm_add_ps(
				_mm_mul_ps(brod3, row3),
				_mm_mul_ps(brod4, row4)));
		_mm_store_ps(&mat.a[4 * i], row);
	}
	return mat;
}

vec3 mat4_mul_vec3(mat4 m, vec3 v) {
	vec4 vec = mat4_mul_vec4(m, (vec4) { v.x, v.y, v.z, 1.0f });
	return (vec3) { vec.x / vec.w, vec.y / vec.w, vec.z / vec.w };
}

vec4 mat4_mul_vec4(mat4 m, vec4 v) {
	__m128 x = _mm_set_ps1(v.x);
	__m128 y = _mm_set_ps1(v.y);
	__m128 z = _mm_set_ps1(v.z);
	__m128 w = _mm_set_ps1(v.w);

	__m128 xs = _mm_set_ps(m.a41, m.a31, m.a21, m.a11);
	__m128 ys = _mm_set_ps(m.a42, m.a32, m.a22, m.a12);
	__m128 zs = _mm_set_ps(m.a43, m.a33, m.a23, m.a13);
	__m128 ws = _mm_set_ps(m.a44, m.a34, m.a24, m.a14);

	xs = _mm_mul_ps(x, xs);
	ys = _mm_mul_ps(y, ys);
	zs = _mm_mul_ps(z, zs);
	ws = _mm_mul_ps(w, ws);

	vec4 vec;
	_mm_store_ps((float*)&vec, _mm_add_ps(_mm_add_ps(xs, ys), _mm_add_ps(zs, ws)));
	return vec;
}

mat4 mat4_perspective0(float fov, float a, float n, float f, int lhc) {
	float thf = tanf((fov / 2.0f) * (3.14159265359f / 180.0f));
	float A, B, C, D, E;
	a = 1 / a;

	if (lhc == 1) {
		A = a * 1.0f / thf;
		B = 1 / thf;
		C = f / (f - n);
		D = 1;
		E = -n * f / (f - n);
	} else {
		A = a * 1.0f / thf;
		B = 1 / thf;
		C = f / (f - n);
		D = -1;
		E = n * f / (f - n);
	}

	return (mat4) {
		A, 0, 0, 0,
		0, B, 0, 0,
		0, 0, C, E,
		0, 0, D, 0
	};
}

mat4 mat4_perspective1(float fov, float a, float n, float f, int lhc) {
	float thf = tanf((fov / 2.0f) * (3.14159265359f / 180.0f));
	float A, B, C, D, E;
	a = 1 / a;

	if (lhc == 1) {
		A = a * 1.0f / thf;
		B = 1 / thf;
		C = -(f + n) / (f - n);
		D = 1;
		E = 2 * f * n / (f - n);
	} else {
		A = a * 1.0f / thf;
		B = 1 / thf;
		C = -(f + n) / (f - n);
		D = -1;
		E = -2 * f * n / (f - n);
	}

	return (mat4) {
		A, 0, 0, 0,
		0, B, 0, 0,
		0, 0, C, E,
		0, 0, D, 0
	};
}

mat4 mat4_ortho0(float left, float right, float bottom, float top, float near, float far, int lhc) {
	mat4 mat = mat4_identity();
	mat.a11 = 2.0f / (right - left);
	mat.a22 = 2.0f / (top - bottom);
	mat.a14 = -(right + left) / (right - left);
	mat.a24 = -(top + bottom) / (top - bottom);
	mat.a34 = -(near) / (far - near);

	if (lhc == 1) {
		mat.a33 = 1.0f / (far - near);
	} else {
		mat.a33 = -1.0f / (far - near);
	}
	return mat;
}

mat4 mat4_ortho1(float left, float right, float bottom, float top, float near, float far, int lhc) {
	mat4 mat = mat4_identity();
	mat.a11 = 2.0f / (right - left);
	mat.a22 = 2.0f / (top - bottom);
	mat.a14 = -(right + left) / (right - left);
	mat.a24 = -(top + bottom) / (top - bottom);
	mat.a34 = -(far + near) / (far - near);

	if (lhc == 1) {
		mat.a33 = -2.0f / (far - near);
	} else {
		mat.a33 = 2.0f / (far - near);
	}
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