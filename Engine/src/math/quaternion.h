#pragma once
#include "math_types.h"

quaternion quaternion_zero();

quaternion euler_to_quaternion(vec3 euler);
vec3 quaternion_to_euler(quaternion value);
mat4 quaternion_to_mat4(quaternion value);
