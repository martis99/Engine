#include "pch.h"
#include "app.h"

Window* window_create(Window* window, WindowSettings settings) {
	window->window = awindow_create(settings.width, settings.height);
	window->settings = settings;
	return window;
}

void window_delete(Window* window) {
	awindow_delete(window->window);
}