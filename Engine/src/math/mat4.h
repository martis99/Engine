#pragma once
#include "math_types.h"

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

mat4 mat4_perspective0(float fovy, float aspect, float zNear, float zFar);
mat4 mat4_perspective1(float fovy, float aspect, float zNear, float zFar);
mat4 mat4_ortho0(float left, float right, float bottom, float top, float near, float far);
mat4 mat4_ortho1(float left, float right, float bottom, float top, float near, float far);

mat4 mat4_look_at_lh(vec3 eye, vec3 center, vec3 up);
mat4 mat4_look_at_rh(vec3 eye, vec3 center, vec3 up);