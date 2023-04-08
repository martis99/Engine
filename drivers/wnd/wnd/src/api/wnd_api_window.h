#pragma once
#include "wnd_api_types.h"

AWindow *awindow_create(AWindowCallbacks *callbacks, ACursor *cursor, int width, int height);
void awindow_delete(AWindow *window);

void *awindow_get_window(AWindow *window);

void awindow_set_title(AWindow *window, const char *title);
int awindow_poll_events(AWindow *window);

void awindow_message_box(AWindow *window, const char *text, const char *caption);
void awindow_message_boxw(AWindow *window, const wchar *text, const wchar *caption);

void awindow_close(AWindow *window);
