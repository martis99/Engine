#include "pch.h"
#include "app.h"

static App* create_app(App* app, int width, int height) {
	if (stats_create(&app->stats) == NULL) {
		log_error("Failed to create stats");
		return NULL;
	}

	WindowSettings window_settings = { 0 };
	window_settings.width = width;
	window_settings.height = height;

	if (window_create(&app->window, window_settings) == NULL) {
		log_error("Failed to create window");
		return NULL;
	}

	return app;
}

static void delete_app(App* app) {
	window_delete(&app->window);
	stats_delete(&app->stats);
}

static void main_loop(App* app) {

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