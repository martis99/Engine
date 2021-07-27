#pragma once
#include "api/astructs.h"

AWindow* awindow_create(AWindowCallbacks* callbacks, ACursor* cursor, int width, int height);
void awindow_delete(AWindow* window);

void* awindow_get_window(AWindow* window);

void awindow_set_title(AWindow* window, const char* title);
int awindow_poll_events(AWindow* window);
void awindow_close(AWindow* window);