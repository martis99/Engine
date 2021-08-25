#include "pch.h"
#include "camera.h"

#include "input/mouse.h"

static void update_view_projection(Camera* camera) {
	camera->view_projection = mat4_identity();
	camera->view_projection = mat4_mul(camera->projection, camera->view_projection);
	camera->view_projection = mat4_mul(camera->view, camera->view_projection);
}

static void update_view(Camera* camera) {
	camera->view = mat4_identity();
	camera->view = mat4_mul(camera->rotation_matrix, camera->view);
	camera->view = mat4_mul(camera->translation_matrix, camera->view);
	update_view_projection(camera);
}

static void update_projection(Camera* camera) {
	camera->projection = mat4_perspective(deg2rad(camera->settings.fov), camera->settings.width / camera->settings.height, camera->settings.z_near, camera->settings.z_far);
	update_view_projection(camera);
}

static void update_translation_matrix(Camera* camera) {
	camera->translation_matrix = mat4_translation(camera->position);
	update_view(camera);
}

static void update_rotation_matrix(Camera* camera) {
	mat4 rotx = mat4_rotation_x(deg2rad(camera->rotation.x));
	mat4 roty = mat4_rotation_y(deg2rad(camera->rotation.y));
	mat4 rotz = mat4_rotation_z(deg2rad(camera->rotation.z));

	mat4 rot = mat4_identity();;
	rot = mat4_mul(rotx, rot);
	rot = mat4_mul(roty, rot);
	rot = mat4_mul(rotz, rot);
	camera->rotation_matrix = rot;
	update_view(camera);
}

static void update_position(Camera* camera) {
	update_translation_matrix(camera);
	update_view(camera);
}

static void update_directions(Camera* camera) {
	camera->right = mat4_mul_vec3(mat4_invert(camera->rotation_matrix), vec3_right());
	camera->up = mat4_mul_vec3(mat4_invert(camera->rotation_matrix), vec3_up());
	camera->front = mat4_mul_vec3(mat4_invert(camera->rotation_matrix), vec3_front());
}

static void update_rotation(Camera* camera) {
	update_rotation_matrix(camera);
	update_directions(camera);
}

Camera* camera_create(Camera* camera, vec3 position, vec3 rotation, CameraSettings settings) {
	camera->settings = settings;

	camera->position = position;
	camera->rotation = rotation;

	update_position(camera);
	update_rotation(camera);
	update_projection(camera);

	return camera;
}

void camera_mouse_wheel(Camera* camera, float delta) {
	if (delta != 0) {
		camera->position = vec3_add(camera->position, vec3_mulf(camera->front, camera->settings.move_speed * delta));
		update_position(camera);
	}
}

void camera_mouse_moved(Camera* camera, float x, float y) {
	if (is_mouse_right_pressed() == 1 && (x != 0.0f || y != 0.0f)) {
		camera->rotation.x += y * camera->settings.rotate_speed;
		camera->rotation.y += x * camera->settings.rotate_speed;
		update_rotation(camera);
	} else if (is_mouse_middle_pressed() == 1) {
		if (x != 0.0f) {
			camera->position = vec3_add(camera->position, vec3_mulf(camera->right, camera->settings.move_speed * x));
			update_position(camera);
		}
		if (y != 0.0f) {
			camera->position = vec3_add(camera->position, vec3_mulf(camera->up, camera->settings.move_speed * -y));
			update_position(camera);
		}
	}
}