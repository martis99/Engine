#pragma once
#include "stats.h"
#include "window/cursor.h"
#include "window/window.h"
#include "window/context.h"
#include "renderer/renderer.h"
#include "scene/scene.h"

typedef struct App {
	Stats stats;
	Cursor cursor;
	Window window;
	Context context;
	Renderer renderer;
	Scene* scene;
} App;

App app;

int app_run();