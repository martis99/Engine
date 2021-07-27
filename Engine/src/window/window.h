#pragma once
#include "structs.h"

Window* window_create(Window* window, WindowSettings settings, AWindowCallbacks* callbacks, Cursor* cursor);
void window_delete(Window* window);

void window_set_title(Window* window, const char* title);
int window_poll_events(Window* window);
void window_close(Window* window);