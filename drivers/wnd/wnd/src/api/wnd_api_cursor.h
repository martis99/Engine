#pragma once
#include "wnd_api_types.h"

ACursor *acursor_create(AWindow *window, byte enabled, LogCallbacks *log);
void acursor_delete(ACursor *cursor);

void acursor_confine(ACursor *cursor);
void acursor_free(ACursor *cursor);
void acursor_hide(ACursor *cursor);
void acursor_show(ACursor *cursor);

void acursor_set_in_window(ACursor *cursor, bool in_window);
bool acursor_get_in_window(ACursor *cursor);

void acursor_set_enabled(ACursor *cursor, bool enabled);
bool acursor_get_enabled(ACursor *cursor);
