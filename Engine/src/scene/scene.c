#include "pch.h"
#include "scene.h"

#include "ecs/system/mesh_renderer.h"

#include "ecs/component/transform.h"
#include "ecs/component/mesh_component.h"

#include "camera.h"

struct Scene {
	Assets assets;
	Ecs ecs;
	Camera camera;
	Entity container;
	MeshRenderer mesh_renderer;
	mat4 projection;
};

Scene* scene_create(float width, float height) {
	Scene* scene = m_malloc(sizeof(Scene));
	assets_create(&scene->assets);

	if (ecs_create(&scene->ecs, 2, sizeof(Transform), sizeof(MeshComponent)) == NULL) {
		log_error("Failed to create ecs");
	}

	if (mesh_renderer_create(&scene->mesh_renderer, &scene->assets) == NULL) {
		log_error("Failed to create mesh renderer");
	}

	vec4 color_orange = (vec4){ 1.0f, 0.5f, 0.1f, 1.0f };
	vec4 color_white = (vec4){ 1.0f, 1.0f, 1.0f, 1.0f };

	Mesh* mesh_quad = assets_mesh_create(&scene->assets, "mesh_quad");
	mesh_init_quad(mesh_quad);
	Mesh* mesh_cube = assets_mesh_create(&scene->assets, "mesh_cube");
	mesh_init_cube(mesh_cube);

	Image* image_white = assets_image_create(&scene->assets, "image_white", 1, 1, 4);
	uint data = (uint)0xffffffff;
	image_set_data(image_white, (unsigned char*)&data);

	Image* image_container = assets_image_load(&scene->assets, "image_container", "res/images/container.jpg");

	Texture* texture_white = assets_texture_create_from_image(&scene->assets, "texture_white", image_white, F_LINEAR);
	Texture* texture_container = assets_texture_create_from_image(&scene->assets, "texture_container", image_container, F_LINEAR);

	Material* material_container = mesh_renderer_create_material(&scene->mesh_renderer, &scene->assets, "material_container", texture_container, color_white);

	{
		Entity container = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3) { 5.0f, 2.0f, 2.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f });
		MeshComponent mesh = mesh_component_create(mesh_cube, material_container);

		ecs_add(&scene->ecs, container.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, container.id, C_MESH, &mesh);
	}

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
	mesh_renderer_delete(&scene->mesh_renderer);
	ecs_delete(&scene->ecs);
	assets_delete(&scene->assets);
	m_free(scene, sizeof(Scene));
}

void scene_update(Scene* scene, float dt) {

}

void scene_render(Scene* scene, Renderer* renderer) {
	mesh_renderer_render(&scene->mesh_renderer, &scene->ecs, &scene->camera.view_projection);
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