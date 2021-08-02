#pragma once

typedef struct mat4 {
	float a11, a12, a13, a14;
	float a21, a22, a23, a24;
	float a31, a32, a33, a34;
	float a41, a42, a43, a44;
} mat4;

float deg2rad(float deg);

mat4 mat4_identity();

mat4 mat4_scalef(float value);
mat4 mat4_translation(vec3 value);
mat4 mat4_rotation_x(float value);
mat4 mat4_rotation_y(float value);
mat4 mat4_rotation_z(float value);
mat4 mat4_scale(vec3 value);

mat4 mat4_invert(mat4 m);

mat4 mat4_mul(mat4 l, mat4 r);
vec3 mat4_mul_vec3(mat4 m, vec3 v);
vec4 mat4_mul_vec4(mat4 m, vec4 v);

mat4 mat4_perspective(float fovy, float aspect, float zNear, float zFar);
mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far);

mat4 mat4_look_at_lh(vec3 eye, vec3 center, vec3 up);
mat4 mat4_look_at_rh(vec3 eye, vec3 center, vec3 up);