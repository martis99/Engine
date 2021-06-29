#pragma once
#include "stats.h"
#include "window/cursor.h"
#include "window/window.h"

typedef struct App {
	Stats stats;
	Cursor cursor;
	Window window;
} App;

App app;

int app_run();