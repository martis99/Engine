#pragma once
#include "stats.h"
#include "window/window.h"

typedef struct App {
	Stats stats;
	Window window;
} App;

App app;

int app_run();