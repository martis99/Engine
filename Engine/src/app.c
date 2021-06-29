#include "pch.h"
#include "app.h"

#include <time.h>

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

static void loop(App* app, float dt) {

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
			sprintf_s(title, 100, "Engine %u FPS %.2f ms, mem: %u", frames, ms, (uint)app->stats.memory);
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