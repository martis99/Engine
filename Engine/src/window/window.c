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

void window_set_title(Window* window, const char* title) {
	awindow_set_title(window->window, title);
}

int window_poll_events(Window* window) {
	return awindow_poll_events(window->window);
}