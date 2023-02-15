#pragma once

#include "gl/gl_types.h"

#include <Windows.h>

struct AContext {
	HWND window;
	HDC device;
	HGLRC context;
	HMODULE library;
	GLError error;
};
