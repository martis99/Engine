#include "scene.h"

#include "assets/image.h"
#include "input/keyboard.h"
#include "input/keys.h"
#include "input/mouse.h"
#include "math/maths.h"
#include "scene/camera.h"
#include "utils_profiler.h"
#include "utils_tools.h"

#include "ecs/component/constraints.h"
#include "ecs/component/transform.h"
#include "ecs/ecs.h"
#include "ecs/system/constraints_resolver.h"

#include "wnd_cursor.h"
#include "wnd_log.h"
#include "wnd_window.h"

#include "ctx_context.h"
#include "ctx_driver.h"
#include "gfx_driver.h"
#include "gfx_renderer.h"

#include "ecs/system/gfx_instance_renderer.h"
#include "ecs/system/gfx_line_renderer.h"
#include "ecs/system/gfx_mesh_renderer.h"
#include "ecs/system/gfx_sprite_renderer.h"
#include "ecs/system/gfx_text_renderer.h"
#include "model_renderer.h"

#include "ecs/component/gfx_instance_component.h"
#include "ecs/component/gfx_mesh_component.h"
#include "ecs/component/gfx_sprite.h"
#include "ecs/component/gfx_text.h"

#include "assets/gfx_assets.h"
#include "assets/gfx_font.h"
#include "assets/gfx_framebuffer.h"
#include "assets/gfx_material.h"
#include "assets/gfx_uniform_buffer.h"
#include "model.h"

#include "print.h"

#include <stdlib.h>
#include <string.h>

#define C_TRANSFORM   0
#define C_MESH	      1
#define C_SPRITE      2
#define C_TEXT	      3
#define C_CONSTRAINTS 4
#define C_INSTANCE    5
#define C_MODEL	      6

typedef struct Scene {
	Window window;
	Cursor cursor;
	Context context;
	Renderer renderer;
	bool wireframe;
	bool cull_back;
	Framebuffer framebuffer;
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
	ModelRenderer model_renderer;
	mat4 projection;
	UniformBuffer u_camera;
	bool profile;
	LogCallbacks log;
	Model models[10];
	int width;
	int height;
	const char *gfx_driver;
} Scene;

static Scene *create_systems(Scene *scene)
{
	if (mesh_renderer_create(&scene->mesh_renderer, &scene->renderer) == NULL) {
		log_error("Failed to create mesh renderer");
		return NULL;
	}

	Transform sprite_transform = transform_create((vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 1.0f, 1.0f, 1.0f });
	if (sprite_renderer_create(&scene->sprite_renderer, &scene->renderer, sprite_transform) == NULL) {
		log_error("Failed to create sprite renderer");
		return NULL;
	}

	Transform text_transform = transform_create((vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 1.0f, 1.0f, 1.0f });
	if (text_renderer_create(&scene->text_renderer, &scene->renderer, text_transform) == NULL) {
		log_error("Failed to create text renderer");
		return NULL;
	}

	Transform line_transform = transform_create((vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 1.0f, 1.0f, 1.0f });
	if (line_renderer_create(&scene->line_renderer, &scene->renderer, line_transform) == NULL) {
		log_error("Failed to create line renderer");
		return NULL;
	}

	if (instance_renderer_create(&scene->instance_renderer, &scene->renderer) == NULL) {
		log_error("Failed to create instance renderer");
		return NULL;
	}

	if (model_renderer_create(&scene->model_renderer, &scene->renderer) == NULL) {
		log_error("Failed to create model renderer");
		return NULL;
	}
	return scene;
}

#define S_ASSERT(var)        \
	if (var == NULL) {   \
		return NULL; \
	}

static Scene *create_assets(Scene *scene)
{
	assets_create(&scene->assets, &scene->renderer);

	Image *image_gui = assets_image_load(&scene->assets, "imgui", "res/images/gui.png");
	S_ASSERT(image_gui);
	Image *image_mountains = assets_image_load(&scene->assets, "mountains", "res/images/mountains.jpg");
	S_ASSERT(image_mountains);

	Texture *texture_gui = assets_texture_create(&scene->assets, "gui", image_gui, A_CLAMP_TO_EDGE, A_NEAREST);
	S_ASSERT(texture_gui);
	Texture *texture_mountains = assets_texture_create(&scene->assets, "mountains", image_mountains, A_CLAMP_TO_EDGE, A_LINEAR);
	S_ASSERT(texture_mountains);

	EFont *font = assets_font_load(&scene->assets, "font", "res/fonts/ProggyClean.ttf", 13);
	S_ASSERT(font);

	Image *image_white = assets_image_create(&scene->assets, "white", 1, 1, 4);
	S_ASSERT(image_white);
	uint data = (uint)0xffffffff;
	image_set_data(image_white, (unsigned char *)&data);
	Image *image_container = assets_image_load(&scene->assets, "container", "res/images/container.jpg");
	S_ASSERT(image_container);

	Texture *texture_white = assets_texture_create(&scene->assets, "white", image_white, A_CLAMP_TO_EDGE, A_NEAREST);
	S_ASSERT(texture_white);
	Texture *texture_container = assets_texture_create(&scene->assets, "container", image_container, A_CLAMP_TO_EDGE, A_LINEAR);
	S_ASSERT(texture_container);

	vec4 color_white  = { 1.0f, 1.0f, 1.0f, 1.0f };
	vec4 color_orange = { 1.0f, 0.5f, 0.2f, 1.0f };

	Material *material_orange = assets_material_create(&scene->assets, "orange", &scene->mesh_renderer.shader);
	S_ASSERT(material_orange);
	material_add_texture(material_orange, texture_white);
	material_set_ps_value(material_orange, 0, &color_orange);
	Material *material_container = assets_material_create(&scene->assets, "container", &scene->mesh_renderer.shader);
	S_ASSERT(material_container);
	material_add_texture(material_container, texture_container);
	material_set_ps_value(material_container, 0, &color_white);

	Material *material_orange_inst = assets_material_create(&scene->assets, "orange_inst", &scene->instance_renderer.shader);
	S_ASSERT(material_orange_inst);
	material_add_texture(material_orange_inst, texture_white);
	material_set_ps_value(material_orange_inst, 0, &color_orange);
	Material *material_container_inst = assets_material_create(&scene->assets, "container_inst", &scene->instance_renderer.shader);
	S_ASSERT(material_container_inst);
	material_add_texture(material_container_inst, texture_container);
	material_set_ps_value(material_container_inst, 0, &color_white);

	Mesh *mesh_cube = assets_mesh_create_cube(&scene->assets, "cube", &scene->mesh_renderer.shader);
	S_ASSERT(mesh_cube);
	Mesh *mesh_cube_inst = assets_mesh_create_cube(&scene->assets, "cube_inst", &scene->instance_renderer.shader);
	S_ASSERT(mesh_cube_inst);

	Model *container = model_load(&scene->models[0], &scene->renderer, "res/models/container/", "container.mdl", &scene->model_renderer.shader, 0, 1);
	S_ASSERT(container);
	Model *backpack = model_load(&scene->models[1], &scene->renderer, "res/models/backpack/", "backpack.mdl", &scene->model_renderer.shader, 0, 1);
	S_ASSERT(backpack);
	Model *nanosuit = model_load(&scene->models[2], &scene->renderer, "res/models/nano_textured/", "nanosuit.mdl", &scene->model_renderer.shader, 0, 1);
	S_ASSERT(nanosuit);
	return scene;
}

static void create_entities2d(Scene *scene)
{
	Texture *texture_gui	   = assets_texture_get(&scene->assets, "gui");
	Texture *texture_mountains = assets_texture_get(&scene->assets, "mountains");

	EFont *font = assets_font_get(&scene->assets, "font");

	Entity panel = ecs_entity(&scene->ecs);
	scene->panel = panel;
	{
		Transform transform	= transform_create((vec3){ 0, 0, 0 }, (vec3){ 0, 0, 0 }, (vec3){ 350, 400, 1 });
		Sprite sprite		= sprite_create(texture_gui, (vec4){ 1.0f, 1.0f, 1.0f, 1.0f }, (vec4){ 1, 10, 18, 10 });
		Constraints constraints = constraints_create((vec3){ 0, 0, 0 });

		ecs_add(&scene->ecs, panel.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, panel.id, C_CONSTRAINTS, &constraints);
		ecs_add(&scene->ecs, panel.id, C_SPRITE, &sprite);
	}
	{
		Entity entity		= ecs_entity(&scene->ecs);
		Transform transform	= transform_create((vec3){ 0, 10, 0 }, (vec3){ 0, 0, 0 }, (vec3){ 0, 50, 1 });
		Constraints constraints = constraints_create((vec3){ -1, -1, -1 });
		constraints.b		= constraint_create(panel, 0, 1);
		constraints.l		= constraint_create(panel, 0, 5);
		constraints.r		= constraint_create(panel, 1, 5);
		constraints.u		= constraint_create(panel, 0, 3);

		Text text = text_create("The quick brown fox jumps over the lazy dog", font, (vec4){ 1, 1, 1, 1 });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_CONSTRAINTS, &constraints);
		ecs_add(&scene->ecs, entity.id, C_TEXT, &text);
	}
	{
		Entity entity = ecs_entity(&scene->ecs);
		Transform transform =
			transform_create((vec3){ 10, 500, 0 }, (vec3){ 0, 0, 0 }, (vec3){ (float)texture_mountains->width / 8, (float)texture_mountains->height / 8, 1 });
		Sprite sprite = sprite_create(texture_mountains, (vec4){ 1, 1, 1, 1 }, (vec4){ 0, 0, 0, 0 });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_SPRITE, &sprite);
	}
	{
		Entity entity	    = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3){ 200, 500, 0 }, (vec3){ 0, 0, 0 },
						       (vec3){ (float)texture_mountains->width / 8, (float)texture_mountains->height / 8, 1 });
		Sprite sprite	    = sprite_create_sub(texture_mountains, (vec4){ 1, 1, 1, 1 }, (vec2i){ 320, 213 }, (vec2i){ 640, 426 });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_SPRITE, &sprite);
	}
}

static void create_entities3d(Scene *scene)
{
	Material *material_orange    = assets_material_get(&scene->assets, "orange");
	Material *material_container = assets_material_get(&scene->assets, "container");

	Material *material_orange_inst	  = assets_material_get(&scene->assets, "orange_inst");
	Material *material_container_inst = assets_material_get(&scene->assets, "container_inst");

	Mesh *mesh_cube	     = assets_mesh_get(&scene->assets, "cube");
	Mesh *mesh_cube_inst = assets_mesh_get(&scene->assets, "cube_inst");

	Model *container = &scene->models[0];
	Model *backpack	 = &scene->models[1];
	Model *nanosuit	 = &scene->models[2];

	{
		Entity entity	    = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 1.0f, 1.0f, 1.0f });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_MODEL, container);
	}
	{
		Entity entity	    = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3){ -5.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 1.0f, 1.0f, 1.0f });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_MODEL, backpack);
	}
	{
		Entity entity	    = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3){ 10.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.4f, 0.4f, 0.4f });

		ecs_add(&scene->ecs, entity.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, entity.id, C_MODEL, nanosuit);
	}

	{
		Entity cube	    = ecs_entity(&scene->ecs);
		Transform transform = transform_create((vec3){ 1.0f, 1.0f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.5f, 0.5f, 0.5f });
		MeshComponent mesh  = mesh_component_create(mesh_cube, material_orange);

		ecs_add(&scene->ecs, cube.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, cube.id, C_MESH, &mesh);
	}
	{
		Entity cube	    = ecs_entity(&scene->ecs);
		scene->cube	    = cube;
		Transform transform = transform_create((vec3){ 2.0f, 2.0f, 2.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.5f, 0.5f, 0.5f });
		MeshComponent mesh  = mesh_component_create(mesh_cube, material_container);

		ecs_add(&scene->ecs, cube.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, cube.id, C_MESH, &mesh);
	}
	{
		Entity cubes		   = ecs_entity(&scene->ecs);
		Transform transform	   = transform_create((vec3){ 0.0f, 0.0f, 50.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.5f, 0.5f, 0.5f });
		InstanceComponent instance = instance_component_create(mesh_cube_inst, material_orange_inst, 10 * 10);
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				Transform transform = transform_create((vec3){ x * 3.0f, y * 3.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.5f, 0.5f, 0.5f });
				instance_component_add(&instance, &transform);
			}
		}

		ecs_add(&scene->ecs, cubes.id, C_TRANSFORM, &transform);
		ecs_add(&scene->ecs, cubes.id, C_INSTANCE, &instance);
	}

	line_renderer_add(&scene->line_renderer, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 1.0f, 0.0f, 0.0f }, (vec4){ 1.0f, 0.0f, 0.0f, 1.0f }, -2);
	line_renderer_add(&scene->line_renderer, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 1.0f, 0.0f }, (vec4){ 0.0f, 1.0f, 0.0f, 1.0f }, -2);
	line_renderer_add(&scene->line_renderer, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 1.0f }, (vec4){ 0.0f, 0.0f, 1.0f, 1.0f }, -2);
}

static void create_camera(Scene *scene, float width, float height, vec3 position, vec3 rotation)
{
	CameraSettings camera_settings = {
		.move_speed   = 0.01f,
		.rotate_speed = 0.15f,
		.width	      = width,
		.height	      = height,
		.fov	      = 45.0f,
		.z_near	      = 1.0f,
		.z_far	      = 1000.0f,
	};

	mat4 projection = renderer_perspective(&scene->renderer, camera_settings.fov, camera_settings.width / camera_settings.height, camera_settings.z_near,
					       camera_settings.z_far);
	camera_create(&scene->camera, position, rotation, camera_settings, projection);

	scene->projection = renderer_ortho(&scene->renderer, 0.0f, 1600.0f, 900.0f, 0.0f, 1000.0f, -1);
}

static void on_msg(void *arg, const char *text)
{
	log_error(text);
}

static void on_err(void *arg, const char *text, const char *caption)
{
	show_error(arg, text, caption);
}

static void on_errw(void *arg, const wchar *text, const wchar *caption)
{
	show_errorw(arg, text, caption);
}

static int create_graphics(Scene *scene, vec3 camera_position, vec3 camera_rotation)
{
	if (context_create(&scene->context, scene->window.window, &scene->log, scene->gfx_driver) == NULL) {
		log_error("Failed to create context");
		return 1;
	}

	if (renderer_create(&scene->renderer, &scene->context, scene->width, scene->height, &scene->log, 1) == NULL) {
		log_error("Failed to create renderer");
		return 1;
	}

	renderer_depth_stencil_set(&scene->renderer, 1, 1);

	AAttachmentDesc attachments[] = {
		{ VEC4F, 0, A_LINEAR, A_REPEAT },
		{ VEC1F, 1, A_LINEAR, A_REPEAT },
	};

	if (framebuffer_create(&scene->framebuffer, &scene->renderer, attachments, sizeof(attachments), scene->width, scene->height) == NULL) {
		log_error("Failed to create framebuffer");
		return 1;
	}

	if (ecs_create(&scene->ecs, 7, sizeof(Transform), sizeof(MeshComponent), sizeof(Sprite), sizeof(Text), sizeof(Constraints), sizeof(InstanceComponent),
		       sizeof(Model)) == NULL) {
		log_error("Failed to create ecs");
		return 1;
	}
	if (create_systems(scene) == NULL) {
		log_error("Failed to create systems");
		return 1;
	}
	if (create_assets(scene) == NULL) {
		log_error("Failed to create assets");
		return 1;
	}

	create_entities2d(scene);
	create_entities3d(scene);

	create_camera(scene, (float)scene->width, (float)scene->height, camera_position, camera_rotation);

	AValue uniforms[] = {
		{ MAT4F, "ViewProjection" },
	};

	ABufferDesc desc = {
		.values	     = uniforms,
		.values_size = sizeof(uniforms),
		.slot	     = 0,
	};

	if (uniformbuffer_create_dynamic(&scene->u_camera, &scene->renderer, &desc) == NULL) {
		log_error("Failed to create camera buffer");
		return 1;
	}

	scene->profile = 0;

	return 0;
}

static void delete_graphics(Scene *scene)
{
	uniformbuffer_delete(&scene->u_camera, &scene->renderer);

	mesh_renderer_delete(&scene->mesh_renderer);
	sprite_renderer_delete(&scene->sprite_renderer);
	text_renderer_delete(&scene->text_renderer);
	line_renderer_delete(&scene->line_renderer);
	QueryResult *qr = ecs_query(&scene->ecs, 1, C_INSTANCE);
	for (uint i = 0; i < qr->count; ++i) {
		instance_component_delete(ecs_get(&scene->ecs, qr->list[i], C_INSTANCE));
	}
	instance_renderer_delete(&scene->instance_renderer);
	model_renderer_delete(&scene->model_renderer);
	ecs_delete(&scene->ecs);
	assets_delete(&scene->assets);
	model_delete(&scene->models[0], &scene->renderer);
	model_delete(&scene->models[1], &scene->renderer);
	model_delete(&scene->models[2], &scene->renderer);
	framebuffer_delete(&scene->framebuffer, &scene->renderer);
	renderer_delete(&scene->renderer);
	context_delete(&scene->context);
}

static void scene_key_pressed(void *priv, byte key)
{
	Scene *scene = priv;
	kb_key_pressed(key);
	switch (key) {
	case K_ESCAPE: window_close(&scene->window); break;
	case K_TAB: scene->wireframe = 1 - scene->wireframe; break;
	case K_F1:
		delete_graphics(scene);
		scene->gfx_driver = ctx_driver_get_name((ctx_driver_get_id(scene->gfx_driver) + 1) % ctx_driver_get_count());
		create_graphics(scene, scene->camera.position, scene->camera.rotation);
		break;
	case 'C': scene->cull_back = 1 - scene->cull_back; break;
	case 'P':
		if (scene->profile == 0) {
			utils_profiler_start("profiler.json");
			scene->profile = 1;
		} else if (scene->profile == 1) {
			utils_profiler_end();
			scene->profile = 0;
		}
		break;
	}
}

static void scene_key_released(void *priv, byte key)
{
	kb_key_released(key);
}

static void scene_mouse_pressed(void *priv, byte button)
{
	Scene *scene = priv;
	ms_button_pressed(button);
	if (button == K_MOUSEL) {
		int x	   = (int)get_mouse_x();
		int y	   = (int)get_mouse_y();
		float entity = 0;
		framebuffer_read_pixel(&scene->framebuffer, &scene->renderer, 1, x, y, &entity);
		printf("%i\n", (int)entity);
	}
}

static void scene_mouse_released(void *priv, byte button)
{
	ms_button_released(button);
}

static void scene_mouse_moved(void *priv, float x, float y)
{
	ms_moved(x, y);
}

static void scene_mouse_moved_delta(void *priv, float dx, float dy)
{
	Scene *scene = priv;
	ms_moved_delta(dx, dy);
	camera_mouse_moved(&scene->camera, dx, dy);
}

static void scene_mouse_wheel(void *priv, float delta)
{
	Scene *scene = priv;
	ms_mouse_wheel(delta);
	camera_mouse_wheel(&scene->camera, delta);
}

static Scene *scene_create(int width, int height)
{
	Scene *scene = m_malloc(sizeof(Scene));

	scene->log.on_msg  = on_msg;
	scene->log.on_err  = on_err;
	scene->log.on_errw = on_errw;
	scene->log.arg	   = &scene->window;

	scene->width	  = width;
	scene->height	  = height;
	scene->gfx_driver = ctx_driver_get_primary();

	AWindowCallbacks callbacks = {
		.key_pressed	   = scene_key_pressed,
		.key_released	   = scene_key_released,
		.mouse_pressed	   = scene_mouse_pressed,
		.mouse_released	   = scene_mouse_released,
		.mouse_moved	   = scene_mouse_moved,
		.mouse_moved_delta = scene_mouse_moved_delta,
		.mouse_wheel	   = scene_mouse_wheel,
		.priv		   = scene,
	};

	WindowSettings window_settings = {
		.width	= scene->width,
		.height = scene->height,
	};

	if (cursor_create(&scene->cursor, &scene->window, 1, &scene->log) == NULL) {
		log_error("Failed to create cursor");
		return NULL;
	}

	if (window_create(&scene->window, window_settings, &callbacks, &scene->cursor, &scene->log) == NULL) {
		log_error("Failed to create window");
		return NULL;
	}

	const vec3 camera_position = { 3.0f, 4.0f, -20.0f };
	const vec3 camera_rotation = { 0.0f, 0.0f, 0.0f };

	scene->wireframe = 0;
	scene->cull_back = 1;

	if (create_graphics(scene, camera_position, camera_rotation)) {
		log_error("Failed to create graphics");
		return NULL;
	}

	return scene;
}

static void calculate_preffered(Transform *transform, Text *text, Constraints *constraints)
{
	float x = 0;
	float y = 0;

	for (size_t i = 0; i < strlen(text->text); i++) {
		FontCharacter fc = font_get_char(text->font, text->text[i]);

		x += fc.advance;
		if (i + 1 < strlen(text->text)) {
			FontCharacter next = font_get_char(text->font, text->text[i + 1]);
			if (constraints->size.x != -1 && x + next.offset.x + next.size.x > constraints->size.x) {
				x = 0;
				y += text->font->line_height;
				if (text->text[i + 1] == ' ') {
					i++;
				}
			}
		}
	}

	transform->scale_pref.x = x + 1;
	transform->scale_pref.y = y + text->font->line_height;
}

static void text_renderer_calculate_preffered(Ecs *ecs)
{
	QueryResult *qr = ecs_query(ecs, 3, C_TRANSFORM, C_TEXT, C_CONSTRAINTS);
	for (uint i = 0; i < qr->count; ++i) {
		Transform *transform	 = (Transform *)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Text *text		 = (Text *)ecs_get(ecs, qr->list[i], C_TEXT);
		Constraints *constraints = (Constraints *)ecs_get(ecs, qr->list[i], C_CONSTRAINTS);
		calculate_preffered(transform, text, constraints);
	}
}

static void scene_update(Scene *scene, float dt)
{
	text_renderer_calculate_preffered(&scene->ecs);
	constraints_resolver_resolve(&scene->ecs, C_TRANSFORM, C_CONSTRAINTS);

	if (is_key_pressed('R')) {
		((Transform *)ecs_get(&scene->ecs, scene->cube.id, C_TRANSFORM))->rotation.y -= 1.0f * dt;
	}
}

static void render_models(ModelRenderer *model_renderer, Ecs *ecs)
{
	model_renderer_begin(model_renderer);
	QueryResult *qr = ecs_query(ecs, 2, C_TRANSFORM, C_MODEL);
	for (uint i = 0; i < qr->count; ++i) {
		Transform *transform = (Transform *)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Model *model	     = (Model *)ecs_get(ecs, qr->list[i], C_MODEL);
		model_renderer_render(model_renderer, qr->list[i], transform, model);
	}
	model_renderer_end(model_renderer);
}

static void render_instances(InstanceRenderer *instance_renderer, Ecs *ecs)
{
	instance_renderer_begin(instance_renderer);
	QueryResult *qr = ecs_query(ecs, 2, C_TRANSFORM, C_INSTANCE);
	for (uint i = 0; i < qr->count; ++i) {
		Transform *transform		      = (Transform *)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		InstanceComponent *instance_component = (InstanceComponent *)ecs_get(ecs, qr->list[i], C_INSTANCE);
		instance_renderer_render(instance_renderer, qr->list[i], transform, instance_component);
	}
	instance_renderer_end(instance_renderer);
}

static void render_meshes(MeshRenderer *mesh_renderer, Ecs *ecs)
{
	mesh_renderer_begin(mesh_renderer);
	QueryResult *qr = ecs_query(ecs, 2, C_TRANSFORM, C_MESH);
	for (uint i = 0; i < qr->count; ++i) {
		Transform *transform	      = (Transform *)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		MeshComponent *mesh_component = (MeshComponent *)ecs_get(ecs, qr->list[i], C_MESH);
		mesh_renderer_render(mesh_renderer, qr->list[i], transform, mesh_component);
	}
	mesh_renderer_end(mesh_renderer);
}

static void render_texts(TextRenderer *text_renderer, Ecs *ecs)
{
	text_renderer_begin(text_renderer);
	QueryResult *qr = ecs_query(ecs, 2, C_TRANSFORM, C_TEXT);
	for (uint i = 0; i < qr->count; ++i) {
		Transform *transform = (Transform *)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Text *text	     = (Text *)ecs_get(ecs, qr->list[i], C_TEXT);
		text_renderer_render(text_renderer, qr->list[i], transform, text);
	}
	text_renderer_end(text_renderer);
}

static void render_sprites(SpriteRenderer *sprite_renderer, Ecs *ecs)
{
	sprite_renderer_begin(sprite_renderer);
	QueryResult *qr = ecs_query(ecs, 2, C_TRANSFORM, C_SPRITE);
	for (uint i = 0; i < qr->count; ++i) {
		Transform *transform = (Transform *)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Sprite *sprite	     = (Sprite *)ecs_get(ecs, qr->list[i], C_SPRITE);
		sprite_renderer_render(sprite_renderer, qr->list[i], transform, sprite);
	}
	sprite_renderer_end(sprite_renderer);
}

static void scene_render(Scene *scene)
{
	renderer_rasterizer_set(&scene->renderer, scene->wireframe, scene->cull_back, scene->renderer.lhc == 1 ? 0 : 1);

	uint targets[] = { 0, 1 };
	framebuffer_bind_render_targets(&scene->framebuffer, &scene->renderer, targets, sizeof(targets));
	float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	framebuffer_clear_attachment(&scene->framebuffer, &scene->renderer, 0, color);
	float entity = -1;
	framebuffer_clear_attachment(&scene->framebuffer, &scene->renderer, 1, &entity);
	float depth = 1.0f;
	framebuffer_clear_depth_attachment(&scene->framebuffer, &scene->renderer, &depth);

	uniformbuffer_bind_vs(&scene->u_camera, &scene->renderer);

	uniformbuffer_set_value(&scene->u_camera, 0, &scene->camera.world_to_screen);
	uniformbuffer_upload(&scene->u_camera, &scene->renderer);

	render_meshes(&scene->mesh_renderer, &scene->ecs);
	render_models(&scene->model_renderer, &scene->ecs);
	render_instances(&scene->instance_renderer, &scene->ecs);
	line_renderer_render(&scene->line_renderer);

	framebuffer_clear_depth_attachment(&scene->framebuffer, &scene->renderer, &depth);
	renderer_rasterizer_set(&scene->renderer, scene->wireframe, scene->cull_back, 0);

	uniformbuffer_set_value(&scene->u_camera, 0, &scene->projection);
	uniformbuffer_upload(&scene->u_camera, &scene->renderer);

	render_texts(&scene->text_renderer, &scene->ecs);
	render_sprites(&scene->sprite_renderer, &scene->ecs);

	renderer_rasterizer_set(&scene->renderer, 0, 1, 0);
	framebuffer_draw(&scene->framebuffer, &scene->renderer, 0);
	framebuffer_unbind_render_targets(&scene->framebuffer, &scene->renderer, targets, sizeof(targets));

	context_swap_buffers(&scene->context);
}

static void loop(Scene *scene, float dt)
{
	scene_update(scene, dt);

	scene->renderer.draw_calls = 0;
	scene_render(scene);
}

static void scene_main_loop(Scene *scene, m_stats_t *mem_stats)
{
	struct timespec last;

	u_getrealtime(&last);

	struct timespec previous = last;
	struct timespec current;
	uint frames = 0;

	while (window_poll_events(&scene->window)) {
		u_getrealtime(&current);

		double elapsed = ((current.tv_sec - last.tv_sec) * 1e6 + (current.tv_nsec - last.tv_nsec) / 1e3) / 1000000;

		if (elapsed > 1) {
			char title[100];
			float ms = (float)elapsed / (float)frames;
			p_sprintf(title, sizeof(title) / sizeof(char), "Engine %s %u FPS %.2f ms, mem: %zd, dc: %i", scene->gfx_driver, frames, ms, mem_stats->mem,
				  scene->renderer.draw_calls);
			window_set_title(&scene->window, title);

			last   = current;
			frames = 0;
		}

		double dt = ((current.tv_sec - previous.tv_sec) * 1e6 + (current.tv_nsec - previous.tv_nsec) / 1e3) / 1000000;
		loop(scene, (float)dt);

		previous = current;
		frames++;
	}
}

static void scene_delete(Scene *scene)
{
	delete_graphics(scene);
	window_delete(&scene->window);
	cursor_delete(&scene->cursor);
	m_free(scene, sizeof(Scene));
}

int scene_run()
{
	m_stats_t m_stats = { 0 };
	m_init(&m_stats);

	if (utils_profiler_create() == NULL) {
		log_error("Failed to create profiler");
		return EXIT_FAILURE;
	}

	ctx_driver_print();
	gfx_driver_print();

	Scene *scene = scene_create(1600, 900);
	if (scene == NULL) {
		log_error("Failed to create scene");
		return EXIT_FAILURE;
	}

	scene_main_loop(scene, &m_stats);

	scene_delete(scene);
	utils_profiler_delete();

	m_print(stdout);

	return EXIT_SUCCESS;
}
