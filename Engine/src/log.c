#include "pch.h"

#ifdef E_WINDOWS
#include <Windows.h>
#endif

void log_info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);
}

void log_error(const char* message) {
#ifdef E_WINDOWS
	MessageBoxA(0, message, "Error", MB_ICONERROR);
#endif
	printf("%s\n", message);
}