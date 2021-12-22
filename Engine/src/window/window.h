#pragma once
#include "types.h"

#include <Windows.h>

Window* window_create(Window* window, WindowSettings settings, AWindowCallbacks* callbacks, Cursor* cursor);
void window_delete(Window* window);

void window_set_title(Window* window, const char* title);
int window_poll_events(Window* window);

void window_message_box(Window* window, const char* text, const char* caption);
void window_message_boxw(Window* window, const wchar* text, const wchar* caption);

void window_close(Window* window);