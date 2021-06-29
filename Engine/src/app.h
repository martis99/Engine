#pragma once
#include "stats.h"
#include "window/cursor.h"
#include "window/window.h"
#include "window/context.h"

typedef struct App {
	Stats stats;
	Cursor cursor;
	Window window;
	Context context;
} App;

App app;

int app_run();