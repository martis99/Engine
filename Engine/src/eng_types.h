#pragma once
#include "eng_common_types.h"
#include "math/math_types.h"
#include "utils/data_types.h"
#include "ecs/ecs_types.h"
#include "assets/assets_types.h"

#include "mem.h"
#include "log.h"

typedef struct CameraSettings {
	float width;
	float height;
	float fov;
	float z_near;
	float z_far;

	float move_speed;
	float rotate_speed;
} CameraSettings;

typedef struct Camera {
	CameraSettings settings;

	vec3 position;
	vec3 rotation;

	vec3 right;
	vec3 up;
	vec3 front;

	mat4 translation_matrix;
	mat4 rotation_matrix;

	mat4 view;
	mat4 projection;
	mat4 view_projection;
} Camera;