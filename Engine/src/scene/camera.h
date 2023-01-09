#pragma once
#include "math/math_types.h"

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
	mat4 world_to_screen;
	mat4 screen_to_world;
} Camera;

Camera* camera_create(Camera* camera, vec3 position, vec3 rotation, CameraSettings settings, mat4 projection);
void camera_mouse_wheel(Camera* camera, float delta);
void camera_mouse_moved(Camera* camera, float x, float y);
void camera_move_forwards(Camera* camera, float speed);
void camera_move_backwards(Camera* camera, float speed);
void camera_move_left(Camera* camera, float speed);
void camera_move_right(Camera* camera, float speed);
void camera_move_up(Camera* camera, float speed);
void camera_move_down(Camera* camera, float speed);
void camera_rotate_left(Camera* camera, float speed);
void camera_rotate_right(Camera* camera, float speed);
void camera_rotate_up(Camera* camera, float speed);
void camera_rotate_down(Camera* camera, float speed);