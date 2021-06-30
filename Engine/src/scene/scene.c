#include "pch.h"
#include "scene.h"

#include "renderer/renderer.h"
#include "assets/mesh.h"
#include "assets/shader.h"

struct Scene {
	Shader shader;
	Mesh mesh;
};

Scene* scene_create(float width, float height) {
	Scene* scene = m_malloc(sizeof(Scene));

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"void main() {\n"
		"	gl_Position = vec4(a_pos, 1.0);\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\0";

	shader_create(&scene->shader, src_vert, src_frag);
	mesh_init_quad(mesh_create(&scene->mesh));
	return scene;
}

void scene_delete(Scene* scene) {
	mesh_delete(&scene->mesh);
	shader_delete(&scene->shader);
	m_free(scene, sizeof(Scene));
}

void scene_update(Scene* scene, float dt) {
	
}

void scene_render(Scene* scene, Renderer* renderer) {
	shader_bind(&scene->shader);
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