#pragma once
#include "stats.h"
#include "window/cursor.h"
#include "window/window.h"
#include "window/context.h"
#include "renderer/renderer.h"

typedef struct App {
	Stats stats;
	Cursor cursor;
	Window window;
	Context context;
	Renderer renderer;
} App;

App app;

int app_run();