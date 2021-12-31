#include "wnd_log.h"
#include "wnd_window.h"

#include "api/wnd_api_log.h"

void show_error(Window* window, const char* text, const char* caption) {
	printf("%s\n", caption);
	printf("%s\n", text);
	window_message_box(window, text, caption);

#ifdef _DEBUG
	debug_break();
#endif
}

void show_errorw(Window* window, const wchar* text, const wchar* caption) {
	wprintf(L"%ls\n", caption);
	wprintf(L"%ls\n", text);

	window_message_boxw(window, text, caption);

#ifdef _DEBUG
	debug_break();
#endif
}