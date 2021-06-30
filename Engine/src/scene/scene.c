#include "pch.h"
#include "scene.h"

#include "renderer/renderer.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "assets/image.h"

struct Scene {
	Shader shader;
	Mesh mesh;
	mat4 projection;
	Material material1;
	mat4 model1;
	Material material2;
	mat4 model2;
	Image image;
};

Scene* scene_create(float width, float height) {
	Scene* scene = m_malloc(sizeof(Scene));

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"uniform mat4 u_model;\n"
		"uniform mat4 u_view_projection;\n"
		"void main() {\n"
		"	gl_Position = u_view_projection * u_model * vec4(a_pos, 1.0);\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 u_color;\n"
		"void main() {\n"
		"	FragColor = u_color;\n"
		"}\0";

	shader_create(&scene->shader, src_vert, src_frag);
	mesh_init_quad(mesh_create(&scene->mesh));

	vec4 color1 = (vec4){ 1.0f, 0.5f, 0.1f, 1.0f };
	material_create(&scene->material1, &scene->shader);
	material_set_vec4f(&scene->material1, "u_color", 1, &color1);
	scene->model1 = mat4_mul(mat4_scale((vec3) { 300.0f, 400.0f, 1.0f }), mat4_mul(quaternion_to_mat4(euler_to_quaternion((vec3) { 0.0f, 0.0f, 0.0f })), mat4_translation((vec3) { 10.0f, 10.0f, 0.0f })));

	vec4 color2 = (vec4){ 1.0f, 0.0f, 0.0f, 1.0f };
	material_create(&scene->material2, &scene->shader);
	material_set_vec4f(&scene->material2, "u_color", 1, &color2);
	scene->model2 = mat4_mul(mat4_scale((vec3) { 100.0f, 200.0f, 1.0f }), mat4_mul(quaternion_to_mat4(euler_to_quaternion((vec3) { 0.0f, 0.0f, 0.0f })), mat4_translation((vec3) { 320.0f, 10.0f, 0.0f })));

	image_load(&scene->image, "res/images/container.jpg");
	scene->projection = mat4_ortho(0.0f, 1600.0f, 900.0f, 0.0f);
	return scene;
}

void scene_delete(Scene* scene) {
	image_delete(&scene->image);
	material_delete(&scene->material1);
	material_delete(&scene->material2);
	mesh_delete(&scene->mesh);
	shader_delete(&scene->shader);
	m_free(scene, sizeof(Scene));
}

void scene_update(Scene* scene, float dt) {

}

void scene_render(Scene* scene, Renderer* renderer) {
	shader_bind(&scene->shader, &scene->projection);
	shader_set_model(&scene->shader, &scene->model1);
	material_bind(&scene->material1);
	mesh_draw(&scene->mesh);
	shader_set_model(&scene->shader, &scene->model2);
	material_bind(&scene->material2);
	mesh_draw(&scene->mesh);
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

}

void scene_mouse_wheel(Scene* scene, float delta) {

}