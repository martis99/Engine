#include "pch.h"

#include "app.h"
#include "utils/utils.h"
#include "window/window.h"

void log_info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);
}

void log_error(const char* text) {
	printf("%s\n", text);
}

void show_error(const char* text, const char* caption) {
	printf("%s\n", caption);
	printf("%s\n", text);
	window_message_box(app_get_window(), text, caption);

#ifdef _DEBUG
	debug_break();
#endif
}

void show_errorw(const wchar* text, const wchar* caption) {
	wprintf(L"%ls\n", caption);
	wprintf(L"%ls\n", text);

	window_message_boxw(app_get_window(), text, caption);

#ifdef _DEBUG
	debug_break();
#endif
}