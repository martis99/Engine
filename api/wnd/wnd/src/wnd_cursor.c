#include "wnd_cursor.h"
#include "api/wnd_api_cursor.h"

Cursor* cursor_create(Cursor* cursor, Window* window, bool enabled, LogCallbacks* log) {
	cursor->cursor = acursor_create(window->window, enabled, log);
	return cursor;
}

void cursor_delete(Cursor* curosr) {
	acursor_delete(curosr->cursor);
}

void cursor_confine(Cursor* cursor) {
	acursor_confine(cursor->cursor);
}

void cursor_free(Cursor* curosr) {
	acursor_free(curosr->cursor);
}

void cursor_hide(Cursor* curosr) {
	acursor_hide(curosr->cursor);
}

void cursor_show(Cursor* curosr) {
	acursor_show(curosr->cursor);
}