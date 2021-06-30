#include "pch.h"
#include "scene.h"

#include "assets/assets.h"

#include "camera.h"

struct Scene {
	Assets assets;
	Camera camera;
	Shader* shader;
	Mesh* mesh_quad;
	Mesh* mesh_cube;
	Material* material_orange;
	Material* material_container;
	mat4 model_orange;
	mat4 model_container;
	mat4 projection;
};

Scene* scene_create(float width, float height) {
	Scene* scene = m_malloc(sizeof(Scene));
	assets_create(&scene->assets);

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"layout (location = 1) in vec2 a_tex_coord;\n"
		"out vec2 v_tex_coord;\n"
		"uniform mat4 u_model;\n"
		"uniform mat4 u_view_projection;\n"
		"void main() {\n"
		"	gl_Position = u_view_projection * u_model * vec4(a_pos.xy, -a_pos.z, 1.0);\n"
		"	v_tex_coord = a_tex_coord;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 v_tex_coord;\n"
		"uniform vec4 u_color;\n"
		"uniform sampler2D u_texture;\n"
		"void main() {\n"
		"	FragColor = texture(u_texture, v_tex_coord) * u_color;\n"
		"}\0";

	scene->shader = assets_shader_create(&scene->assets, "shader", src_vert, src_frag);

	vec4 color_orange = (vec4){ 1.0f, 0.5f, 0.1f, 1.0f };
	vec4 color_white = (vec4){ 1.0f, 1.0f, 1.0f, 1.0f };

	scene->mesh_quad = assets_mesh_create(&scene->assets, "mesh_quad");
	mesh_init_quad(scene->mesh_quad);
	scene->mesh_cube = assets_mesh_create(&scene->assets, "mesh_cube");
	mesh_init_cube(scene->mesh_cube);

	Image* image_white = assets_image_create(&scene->assets, "image_white", 1, 1, 4);
	uint data = (uint)0xffffffff;
	image_set_data(image_white, (unsigned char*)&data);

	Image* image_container = assets_image_load(&scene->assets, "image_container", "res/images/container.jpg");

	Texture* texture_white = assets_texture_create_from_image(&scene->assets, "texture_white", image_white, F_LINEAR);
	Texture* texture_container = assets_texture_create_from_image(&scene->assets, "texture_container", image_container, F_LINEAR);

	scene->material_orange = assets_material_create(&scene->assets, "material_orange", scene->shader);
	material_add_texture(scene->material_orange, texture_white);
	material_set_vec4f(scene->material_orange, "u_color", 1, &color_orange);

	scene->material_container = assets_material_create(&scene->assets, "material_container", scene->shader);
	material_add_texture(scene->material_container, texture_container);
	material_set_vec4f(scene->material_container, "u_color", 1, &color_white);

	scene->model_orange = mat4_mul(mat4_scale((vec3) { 300.0f, 400.0f, 1.0f }), mat4_mul(quaternion_to_mat4(euler_to_quaternion((vec3) { 0.0f, 0.0f, 0.0f })), mat4_translation((vec3) { 10.0f, 10.0f, 0.0f })));
	scene->model_container = mat4_mul(mat4_scale((vec3) { 1.0f, 1.0f, 1.0f }), mat4_mul(quaternion_to_mat4(euler_to_quaternion((vec3) { 0.0f, 0.0f, 0.0f })), mat4_translation((vec3) { 4.0f, 2.0f, -2.0f })));

	CameraSettings camera_settings = { 0 };
	camera_settings.move_speed = 0.01f;
	camera_settings.rotate_speed = 0.15f;
	camera_settings.width = width;
	camera_settings.height = height;
	camera_settings.fov = 45.0f;
	camera_settings.z_near = 0.001f;
	camera_settings.z_far = 1000.0f;

	const vec3 camera_position = { -3.0f, -2.0f, -5.0f };
	const vec3 camera_rotation = { 0.0f, 0.0f, 0.0f };
	camera_create(&scene->camera, camera_position, camera_rotation, camera_settings);

	scene->projection = mat4_ortho(0.0f, 1600.0f, 900.0f, 0.0f);
	return scene;
}

void scene_delete(Scene* scene) {
	assets_delete(&scene->assets);
	m_free(scene, sizeof(Scene));
}

void scene_update(Scene* scene, float dt) {

}

void scene_render(Scene* scene, Renderer* renderer) {
	shader_bind(scene->shader, &scene->projection);
	shader_set_model(scene->shader, &scene->model_orange);
	material_bind(scene->material_orange);
	mesh_draw(scene->mesh_quad);

	shader_bind(scene->shader, &scene->camera.view_projection);
	shader_set_model(scene->shader, &scene->model_container);
	material_bind(scene->material_container);
	mesh_draw(scene->mesh_cube);
}

void scene_key_pressed(Scene* scene, byte key) {

}

void scene_key_released(Scene* scene, byte key) {

}

void scene_mouse_pressed(Scene* scene, byte button) {

}

void scene_mouse_released(Scene* scene, byte button) {

}

void scene_mouse_moved(Scene* scene, float x, float y) {

}

void scene_mouse_moved_delta(Scene* scene, float dx, float dy) {
	camera_mouse_moved(&scene->camera, dx, dy);
}

void scene_mouse_wheel(Scene* scene, float delta) {
	camera_mouse_wheel(&scene->camera, delta);
}