#include "camera.h"

#include "input/mouse.h"
#include "math/maths.h"

static void update_view_projection(Camera *camera)
{
	camera->world_to_screen = mat4_mul(camera->projection, camera->view);
	camera->screen_to_world = mat4_invert(camera->world_to_screen);
}

static void update_view(Camera *camera)
{
	camera->view = mat4_mul(camera->rotation_matrix, camera->translation_matrix);
	update_view_projection(camera);
}

static void update_translation_matrix(Camera *camera)
{
	camera->translation_matrix = mat4_translation(vec3_mulf(camera->position, -1));
	update_view(camera);
}

static void update_rotation_matrix(Camera *camera)
{
	camera->rotation_matrix = mat4_rotation(vec3_mulf(camera->rotation, -1 * 3.14159265359f / 180));
	update_view(camera);
}

static void update_position(Camera *camera)
{
	update_translation_matrix(camera);
	update_view(camera);
}

static void update_directions(Camera *camera)
{
	mat4 tr	      = mat4_transpose(camera->rotation_matrix);
	camera->right = mat4_mul_vec3(tr, vec3_right());
	camera->up    = mat4_mul_vec3(tr, vec3_up());
	camera->front = mat4_mul_vec3(tr, vec3_front());
}

static void update_rotation(Camera *camera)
{
	update_rotation_matrix(camera);
	update_directions(camera);
}

Camera *camera_create(Camera *camera, vec3 position, vec3 rotation, CameraSettings settings, mat4 projection)
{
	camera->settings = settings;

	camera->position = position;
	camera->rotation = rotation;

	camera->translation_matrix = mat4_translation(vec3_mulf(camera->position, -1));
	camera->rotation_matrix	   = mat4_rotation(vec3_mulf(camera->rotation, -1 * degtorad()));
	camera->projection	   = projection;
	update_view(camera);
	update_directions(camera);
	return camera;
}

void camera_mouse_wheel(Camera *camera, float delta)
{
	if (delta != 0) {
		camera->position = vec3_add(camera->position, vec3_mulf(camera->front, camera->settings.move_speed * delta));
		update_position(camera);
	}
}

void camera_mouse_moved(Camera *camera, float x, float y)
{
	if (is_mouse_right_pressed() == 1 && (x != 0.0f || y != 0.0f)) {
		camera->rotation.x -= y * camera->settings.rotate_speed;
		camera->rotation.y -= x * camera->settings.rotate_speed;
		update_rotation(camera);
	} else if (is_mouse_middle_pressed() == 1) {
		if (x != 0.0f) {
			camera_move_left(camera, camera->settings.move_speed * x);
		}
		if (y != 0.0f) {
			camera_move_up(camera, camera->settings.move_speed * y);
		}
	}
}

void camera_move_forwards(Camera *camera, float speed)
{
	camera->position = vec3_add(camera->position, vec3_mulf(camera->front, speed));
	update_position(camera);
}

void camera_move_backwards(Camera *camera, float speed)
{
	camera_move_forwards(camera, -speed);
}

void camera_move_left(Camera *camera, float speed)
{
	camera_move_right(camera, -speed);
}

void camera_move_right(Camera *camera, float speed)
{
	camera->position = vec3_add(camera->position, vec3_mulf(camera->right, speed));
	update_position(camera);
}

void camera_move_up(Camera *camera, float speed)
{
	camera->position = vec3_add(camera->position, vec3_mulf(camera->up, speed));
	update_position(camera);
}

void camera_move_down(Camera *camera, float speed)
{
	camera_move_up(camera, -speed);
}

void camera_rotate_left(Camera *camera, float speed)
{
	camera->rotation.y += speed;
	update_rotation(camera);
}

void camera_rotate_right(Camera *camera, float speed)
{
	camera_rotate_left(camera, -speed);
}

void camera_rotate_up(Camera *camera, float speed)
{
	camera->rotation.x += speed;
	update_rotation(camera);
}

void camera_rotate_down(Camera *camera, float speed)
{
	camera_rotate_up(camera, -speed);
}
