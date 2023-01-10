#pragma once
#include "api/wnd_api_types.h"

typedef struct WindowSettings {
	int width;
	int height;
} WindowSettings;

typedef struct Window {
	AWindow *window;
	WindowSettings settings;
} Window;

typedef struct Cursor {
	ACursor *cursor;
} Cursor;
