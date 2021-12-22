#pragma once
#include "eng_common_types.h"

void log_info(const char* format, ...);
void log_error(const char* text);
void show_error(const char* text, const char* caption);
void show_errorw(const wchar* text, const wchar* caption);