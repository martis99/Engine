#include "pch.h"

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

void log_msg(LogCallbacks* log, const char* text) {
	log->on_msg(log->arg, text);
}

void log_err(LogCallbacks* log, const char* text, const char* caption) {
	log->on_err(log->arg, text, caption);
}

void log_errw(LogCallbacks* log, const wchar* text, const wchar* caption) {
	log->on_errw(log->arg, text, caption);
}