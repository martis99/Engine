#include "pch.h"
#include "app.h"

static App* create_app(App* app) {
	return app;
}

static void delete_app(App* app) {
	
}

static void main_loop(App* app) {

}

int app_run() {
	if (create_app(&app) == NULL) {
		log_error("Failed to create app");
		return EXIT_FAILURE;
	}

	main_loop(&app);

	delete_app(&app);
	return EXIT_SUCCESS;
}