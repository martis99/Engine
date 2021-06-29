#pragma once
#include "stats.h"

typedef struct App {
	Stats stats;
} App;

App app;

int app_run();