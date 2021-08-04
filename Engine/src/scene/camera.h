#pragma once
#include "types.h"

Camera* camera_create(Camera* camera, vec3 position, vec3 rotation, CameraSettings settings);
void camera_mouse_wheel(Camera* camera, float delta);
void camera_mouse_moved(Camera* camera, float x, float y);