#pragma once
#include "api/wnd/acursor.h"

typedef struct Window Window;

typedef struct Cursor {
	ACursor* cursor;
} Cursor;

Cursor* cursor_create(Cursor* cursor, Window* window, bool enabled);
void cursor_delete(Cursor* cursor);

void cursor_confine(Cursor* cursor);
void cursor_free(Cursor* cursor);
void cursor_hide(Cursor* cursor);
void cursor_show(Cursor* cursor);