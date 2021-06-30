#include "pch.h"
#include "app.h"

#include <time.h>

#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/keys.h"

static void key_pressed(byte key) {
	kb_key_pressed(key);
	scene_key_pressed(app.scene, key);

	switch (key) {
	case K_ESCAPE:
		window_close(&app.window);
		break;
	case K_TAB:
		renderer_toggle_fireframe(&app.renderer);
		break;
	case 'C':
		renderer_toggle_backface_culling(&app.renderer);
		break;
	}
}

static void key_released(byte key) {
	kb_key_released(key);
	scene_key_released(app.scene, key);
}

static void mouse_pressed(byte button) {
	ms_button_pressed(button);
	scene_mouse_pressed(app.scene, button);
}

static void mouse_released(byte button) {
	ms_button_released(button);
	scene_mouse_released(app.scene, button);
}

static void mouse_moved(float x, float y) {
	ms_moved(x, y);
	scene_mouse_moved(app.scene, x, y);
}

static void mouse_moved_delta(float dx, float dy) {
	ms_moved_delta(dx, dy);
	scene_mouse_moved_delta(app.scene, dx, dy);
}

static void mouse_wheel(float delta) {
	ms_mouse_wheel(delta);
	scene_mouse_wheel(app.scene, delta);
}

static App* create_app(App* app, int width, int height) {
	if (stats_create(&app->stats) == NULL) {
		log_error("Failed to create stats");
		return NULL;
	}

	if (cursor_create(&app->cursor, &app->window, 1) == NULL) {
		log_error("Failed to create cursor");
		return NULL;
	}

	WindowSettings window_settings = { 0 };
	window_settings.width = width;
	window_settings.height = height;

	AWindowCallbacks callbacks;
	callbacks.key_pressed = key_pressed;
	callbacks.key_released = key_released;
	callbacks.mouse_pressed = mouse_pressed;
	callbacks.mouse_released = mouse_released;
	callbacks.mouse_moved = mouse_moved;
	callbacks.mouse_moved_delta = mouse_moved_delta;
	callbacks.mouse_wheel = mouse_wheel;

	if (window_create(&app->window, window_settings, &callbacks, &app->cursor) == NULL) {
		log_error("Failed to create window");
		return NULL;
	}

	if (context_create(&app->context, &app->window) == NULL) {
		log_error("Failed to create context");
		return NULL;
	}

	if (renderer_create(&app->renderer) == NULL) {
		log_error("Failed to create renderer");
		return NULL;
	}

	app->scene = scene_create((float)width, (float)height);
	if (app->scene == NULL) {
		log_error("Failed to create scene");
		return NULL;
	}

	return app;
}


static void delete_app(App* app) {
	scene_delete(app->scene);
	renderer_delete(&app->renderer);
	context_delete(&app->context);
	window_delete(&app->window);
	cursor_delete(&app->cursor);
	stats_delete(&app->stats);
}

static void loop(App* app, float dt) {
	scene_update(app->scene, dt);

	renderer_begin(&app->renderer);
	scene_render(app->scene, &app->renderer);
	renderer_end(&app->renderer);

	context_swap_buffers(&app->context);
}

static void main_loop(App* app) {
	clock_t last = clock();
	clock_t previous = last;
	uint frames = 0;

	while (window_poll_events(&app->window)) {
		clock_t current = clock();
		clock_t elapsed = current - last;

		if (elapsed > CLOCKS_PER_SEC) {
			char title[100];
			float ms = elapsed / (float)frames;
			sprintf_s(title, 100, "Engine %u FPS %.2f ms, mem: %u, dc: %i", frames, ms, (uint)app->stats.memory, app->stats.draw_calls);
			window_set_title(&app->window, title);

			last = current;
			frames = 0;
		}

		float dt = (current - previous) / (float)CLOCKS_PER_SEC;
		loop(app, dt);

		previous = current;
		frames++;
	}
}

int app_run() {
	if (create_app(&app, 1600, 900) == NULL) {
		log_error("Failed to create app");
		return EXIT_FAILURE;
	}

	main_loop(&app);

	delete_app(&app);
	return EXIT_SUCCESS;
}