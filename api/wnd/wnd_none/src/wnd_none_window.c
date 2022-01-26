#include "api/wnd_api_window.h"
#include "api/wnd_api_cursor.h"

#include  "wnd_none_types.h"

AWindow* awindow_create(AWindowCallbacks* callbacks, ACursor* cursor, int width, int height, LogCallbacks* log) {
	AWindow* window = m_malloc(sizeof(AWindow));
	window->callbacks = *callbacks;
	window->cursor = cursor;
	window->log = log;
	return window;
}

void awindow_delete(AWindow* window) {
	m_free(window, sizeof(AWindow));
}

void* awindow_get_window(AWindow* window) {
	return NULL;
}

void awindow_set_title(AWindow* window, const char* title) {

}

int awindow_poll_events(AWindow* window) {
	return 1;
}

void awindow_message_box(AWindow* window, const char* text, const char* caption) {

}

void awindow_message_boxw(AWindow* window, const wchar* text, const wchar* caption) {

}

void awindow_close(AWindow* window) {

}