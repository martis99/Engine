﻿#include "pch.h"
#include "scene.h"

#include "ecs/system/mesh_renderer.h"
#include "ecs/system/sprite_renderer.h"
#include "ecs/system/text_renderer.h"
#include "ecs/system/constraints_resolver.h"
#include "ecs/system/line_renderer.h"
#include "ecs/system/instance_renderer.h"

#include "ecs/component/transform.h"
#include "ecs/component/mesh_component.h"
#include "ecs/component/constraints.h"
#include "ecs/component/instance_component.h"

#include "camera.h"

#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/keys.h"

struct Scene {
	Renderer* renderer;
	Assets assets;
	Ecs ecs;
	Camera camera;
	Entity panel;
	Entity cube;
	MeshRenderer mesh_renderer;
	SpriteRenderer sprite_renderer;
	TextRenderer text_renderer;
	LineRenderer line_renderer;
	InstanceRenderer instance_renderer;
	mat4 projection;
	UniformBuffer* u_camera;
};

static void create_systems(Scene* scene) {
	if (mesh_renderer_create(&scene->mesh_renderer, &scene->assets) == NULL) {
		log_error("Failed to create mesh renderer");
	}

	Transform sprite_transform = transform_create((vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f });
	if (sprite_renderer_create(&scene->sprite_renderer, &scene->assets, sprite_transform) == NULL) {
		log_error("Failed to create sprite renderer");
	}

	Transform text_transform = transform_create((vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f });
	if (text_renderer_create(&scene->text_renderer, &scene->assets, text_transform) == NULL) {
		log_error("Failed to create text renderer");
	}
	Transform line_transform = transform_create((vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f });
	if (line_renderer_create(&scene->line_renderer, &scene->assets, line_transform) == NULL) {
		log_error("Failed to create line renderer");
	}

	if (instance_renderer_create(&scene->instance_renderer, &scene->assets) == NULL) {
		log_error("Failed to create instance renderer");
	}
}

static void create_assets(Scene* scene) {
	Image* image_white = assets_image_create(&scene->assets, "white", 1, 1, 4);
	uint data = (uint)0xffffffff;
	image_set_data(image_white, (unsigned char*)&data);

	Image* image_container = assets_image_load(&scene->assets, "container", "res/images/container.jpg");
	Image* image_gui = assets_image_load(&scene->assets, "imgui", "res/images/gui.png");
	Image* image_mountains = assets_image_load(&scene->assets, "mountains", "res/images/mountains.jpg");
	Font* font = assets_font_load(&scene->assets, "font", "res/fonts/ProggyClean.ttf", 13);

	Texture* texture_white = assets_texture_create_from_image(&scene->assets, "white", image_white, A_CLAMP_TO_EDGE, A_NEAREST);
	Texture* texture_container = assets_texture_create_from_image(&scene->assets, "container", image_container, A_CLAMP_TO_EDGE, A_LINEAR);
	Texture* texture_gui = assets_texture_create_from_image(&scene->assets, "gui", image_gui, A_CLAMP_TO_EDGE, A_NEAREST);
	Texture* texture_mountains = assets_texture_create_from_image(&scene->assets, "mountains", image_mountains, A_CLAMP_TO_EDGE, A_LINEAR);

	Mesh* mesh_cube = assets_mesh_create(&scene->assets, "cube");
	mesh_init_cube(mesh_cube);
}

static void create_entities2d(Scene* scene) {

	Texture* texture_gui = assets_texture_get(&scene->assets, "gui");
	Texture* texture_mountains = assets_texture_get(&scene->assets, "mountains");

	Font* font = assets_font_get(&scene->assets, "font");

	Entity panel = ecs_entity(&scene->ecs);
	scene->panel = panel;
	{
		Transform transform = transform_create_2d((vec2i) { 10, 10 }, 0.0f, (vec2i) { 350, 400 });
		Sprite sprite = sprite_create(texture_gui, (vec4) { 1.0f, 1.0f, 1.0f, 1.0f }, (vec4) { 1, 10, 18, 10 });
		Constraints constraints = constraints_create(0, 0);

		ecs_add(&scene->ecs, panel.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, panel.id, C_CONSTRAINTS, &constraints);
		ecs_add(&scene->ecs, panel.id, C_SPRITE, &sprite);
	}
	{
		Entity entity = ecs_entity(&scene->ecs);
		Transform transform = transform_create_2d((vec2i) { 0, 10 }, 0.0f, (vec2i) { 500, 50 });
		Constraints constraints = constraints_create(-1, -1);
		constraints_add_b(&constraints, constraint_create(panel, 0, 0));
		constraints_add_l(&constraints, constraint_create(panel, 0, 5));
		constraints_add_r(&constraints, constraint_create(panel, 1, 5));
		constraints_add_u(&constraints, constraint_create(panel, 0, 3));
		Text text = text_create("The quick brown fox jumps over the lazy dog", font, (vec4) { 1, 1, 1, 1 });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_CONSTRAINTS, &constraints);
		ecs_add(&scene->ecs, entity.id, C_TEXT, &text);
	}
	{
		Entity entity = ecs_entity(&scene->ecs);
		Transform transform = transform_create_2d((vec2i) { 10, 500 }, 0.0f, (vec2i) { texture_mountains->width / 8, texture_mountains->height / 8 });
		Sprite sprite = sprite_create(texture_mountains, (vec4) { 1, 1, 1, 1 }, (vec4) { 0, 0, 0, 0 });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_SPRITE, &sprite);
	}
	{
		Entity entity = ecs_entity(&scene->ecs);
		Transform transform = transform_create_2d((vec2i) { 200, 500 }, 0.0f, (vec2i) { texture_mountains->width / 8, texture_mountains->height / 8 });
		Sprite sprite = sprite_create_sub(texture_mountains, (vec4) { 1, 1, 1, 1 }, (vec2i) { 320, 213 }, (vec2i) { 640, 426 });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_SPRITE, &sprite);
	}
}

static void create_entities3d(Scene* scene) {

	Material* material_white = assets_material_get(&scene->assets, "white");
	Material* material_orange = assets_material_get(&scene->assets, "orange");
	Material* material_container = assets_material_get(&scene->assets, "container");

	Material* material_orange_inst = assets_material_get(&scene->assets, "orange_inst");
	Material* material_container_inst = assets_material_get(&scene->assets, "container_inst");

	Mesh* mesh_cube = assets_mesh_get(&scene->assets, "cube");

	{
		Entity cube = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3) { 2.0f, 2.0f, 2.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f });
		MeshComponent mesh = mesh_component_create(mesh_cube, material_orange);

		ecs_add(&scene->ecs, cube.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, cube.id, C_MESH, &mesh);
	}
	{
		Entity cube = ecs_entity(&scene->ecs);
		scene->cube = cube;
		Transform transform = transform_create((vec3) { 5.0f, 2.0f, 2.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f });
		MeshComponent mesh = mesh_component_create(mesh_cube, material_container);

		ecs_add(&scene->ecs, cube.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, cube.id, C_MESH, &mesh);
	}
	{
		Entity cubes = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3) { 0.0f, 0.0f, 10.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.5f, 0.5f, 0.5f });
		InstanceComponent instance = instance_component_create(mesh_cube, material_orange_inst, 10 * 10);
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				Transform transform = transform_create((vec3) { x * 3.0f, y * 3.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f });
				instance_component_add(&instance, &transform);
			}
		}

		ecs_add(&scene->ecs, cubes.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, cubes.id, C_INSTANCE, &instance);
	}

	line_renderer_add(&scene->line_renderer, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 1.0f, 0.0f, 0.0f }, (vec4) { 1.0f, 0.0f, 0.0f, 1.0f }, -1);
	line_renderer_add(&scene->line_renderer, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 1.0f, 0.0f }, (vec4) { 0.0f, 1.0f, 0.0f, 1.0f }, -1);
	line_renderer_add(&scene->line_renderer, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 1.0f }, (vec4) { 0.0f, 0.0f, 1.0f, 1.0f }, -1);
}

static void create_entities(Scene* scene) {
	if (ecs_create(&scene->ecs, 6, sizeof(Transform), sizeof(MeshComponent), sizeof(Sprite), sizeof(Text), sizeof(Constraints), sizeof(InstanceComponent)) == NULL) {
		log_error("Failed to create ecs");
	}

	create_entities2d(scene);
	create_entities3d(scene);
}

static void create_camera(Scene* scene, float width, float height) {
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
}

Scene* scene_create(float width, float height, Renderer* renderer) {
	Scene* scene = m_malloc(sizeof(Scene));
	scene->renderer = renderer;

	assets_create(&scene->assets);
	create_assets(scene);
	create_systems(scene);

	Texture* texture_white = assets_texture_get(&scene->assets, "white");
	Texture* texture_container = assets_texture_get(&scene->assets, "container");

	vec4 color_white = { 1.0f, 1.0f, 1.0f, 1.0f };
	vec4 color_orange = { 1.0f, 0.5f, 0.2f, 1.0f };

	Material* material_white = mesh_renderer_create_material(&scene->mesh_renderer, &scene->assets, "white", texture_white, color_white);
	Material* material_orange = mesh_renderer_create_material(&scene->mesh_renderer, &scene->assets, "orange", texture_white, color_orange);
	Material* material_container = mesh_renderer_create_material(&scene->mesh_renderer, &scene->assets, "container", texture_container, color_white);

	Material* material_orange_inst = instance_renderer_create_material(&scene->instance_renderer, &scene->assets, "orange_inst", texture_white, color_orange);
	Material* material_container_inst = instance_renderer_create_material(&scene->instance_renderer, &scene->assets, "container_inst", texture_container, color_white);

	create_entities(scene);

	create_camera(scene, width, height);

	scene->u_camera = assets_uniform_buffer_create(&scene->assets, "u_camera");
	uniformbuffer_init_dynamic(scene->u_camera, sizeof(mat4));
	uniformbuffer_bind_base(scene->u_camera, 0);
	return scene;
}

void scene_delete(Scene* scene) {
	mesh_renderer_delete(&scene->mesh_renderer);
	sprite_renderer_delete(&scene->sprite_renderer);
	text_renderer_delete(&scene->text_renderer);
	line_renderer_delete(&scene->line_renderer);
	instance_renderer_delete(&scene->instance_renderer, &scene->ecs);
	ecs_delete(&scene->ecs);
	assets_delete(&scene->assets);
	m_free(scene, sizeof(Scene));
}

void scene_update(Scene* scene, float dt) {
	text_renderer_calculate_preffered(&scene->ecs);
	constraints_resolver_resolve(&scene->ecs);

	if (is_key_pressed('R')) {
		((Transform*)ecs_get(&scene->ecs, scene->cube.id, C_TRANSFORM))->rotation.y -= 1.0f * dt;
	}
}

void scene_render(Scene* scene, Renderer* renderer) {
	uniformbuffer_set_data(scene->u_camera, &scene->camera.view_projection, sizeof(mat4));

	mesh_renderer_render(&scene->mesh_renderer, &scene->ecs);
	instance_renderer_render(&scene->instance_renderer, &scene->ecs);
	line_renderer_render(&scene->line_renderer);

	renderer_clear_depth(renderer);

	uniformbuffer_set_data(scene->u_camera, &scene->projection, sizeof(mat4));

	sprite_renderer_render(&scene->sprite_renderer, &scene->ecs);
	text_renderer_render(&scene->text_renderer, &scene->ecs);
}

void scene_key_pressed(Scene* scene, byte key) {

}

void scene_key_released(Scene* scene, byte key) {

}

void scene_mouse_pressed(Scene* scene, byte button) {
	if (button == 0) {
		int entity = renderer_get_mouse_entity(scene->renderer);
		printf("Pressed entity: %i\n", entity);
	}
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