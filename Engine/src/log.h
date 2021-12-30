#pragma once
#include "eng_common_types.h"

void log_info(const char* format, ...);
void log_error(const char* text);

typedef struct LogCallbacks {
	void(*on_msg)(void* arg, const char* text);
	void(*on_err)(void* arg, const char* text, const char* caption);
	void(*on_errw)(void* arg, const wchar* text, const wchar* caption);
	void* arg;
} LogCallbacks;

void log_msg(LogCallbacks* log, const char* text);
void log_err(LogCallbacks* log, const char* text, const char* caption);
void log_errw(LogCallbacks* log, const wchar* text, const wchar* caption);