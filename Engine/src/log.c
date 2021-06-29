#include "pch.h"

void log_info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);
}

void log_error(const char* message) {
	printf("%s\n", message);
}