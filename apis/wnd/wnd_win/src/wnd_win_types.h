#pragma once
#include "api/wnd_api_types.h"

#include <Windows.h>

struct ACursor {
	AWindow* window;
	bool enabled;
	bool in_window;
	LogCallbacks* log;
};

struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
	AWindowCallbacks callbacks;
	ACursor* cursor;
	LogCallbacks* log;
};