#include "pch.h"
#include "app.h"

#include "stats.h"
#include "profiler.h"

#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/keys.h"

#include "scene/scene.h"

#include <time.h>

static App app;

static App* create_app(App* app, int width, int height) {
	if (stats_create(&app->stats) == NULL) {
		log_error("Failed to create stats");
		return NULL;
	}

	app->scene = scene_create(width, height);
	if (app->scene == NULL) {
		log_error("Failed to create scene");
		return NULL;
	}

	return app;
}


static void delete_app(App* app) {
	scene_delete(app->scene);
	stats_delete(&app->stats);
}

static void main_loop(App* app) {
	scene_main_loop(app->scene);
}

int app_run() {
	mem_init();

	if (profiler_create() == NULL) {
		log_error("Failed to create profiler");
		return EXIT_FAILURE;
	}

	if (create_app(&app, 1600, 900) == NULL) {
		log_error("Failed to create app");
		return EXIT_FAILURE;
	}

	main_loop(&app);

	delete_app(&app);

	profiler_delete();
	return EXIT_SUCCESS;
}

void app_exit() {
	scene_exit(app.scene);
}

Stats* app_get_stats() {
	return &app.stats;
}