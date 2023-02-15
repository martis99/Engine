#include "api/wnd_api_cursor.h"
#include "api/wnd_api_window.h"

#include "wnd_none_types.h"

ACursor *acursor_create(AWindow *window, byte enabled, LogCallbacks *log)
{
	ACursor *cursor = m_malloc(sizeof(ACursor));
	cursor->window	= window;
	cursor->enabled = enabled;
	cursor->log	= log;
	return cursor;
}

void acursor_delete(ACursor *cursor)
{
	m_free(cursor, sizeof(ACursor));
}

void acursor_confine(ACursor *cursor)
{
}

void acursor_free(ACursor *cursor)
{
}

void acursor_hide(ACursor *cursor)
{
}

void acursor_show(ACursor *cursor)
{
}

void acursor_set_in_window(ACursor *cursor, bool in_window)
{
	cursor->in_window = in_window;
}

bool acursor_get_in_window(ACursor *cursor)
{
	return cursor->in_window;
}

void acursor_set_enabled(ACursor *cursor, bool enabled)
{
	cursor->enabled = enabled;
}

bool acursor_get_enabled(ACursor *cursor)
{
	return cursor->enabled;
}
