#pragma once
#include "api/wnd/awindow.h"

typedef struct WindowSettings {
	int width;
	int height;
} WindowSettings;

typedef struct Window {
	AWindow* window;
	WindowSettings settings;
} Window;

Window* window_create(Window* window, WindowSettings settings);
void window_delete(Window* window);