#pragma once

#include "gl_ctx/gl_ctx_types.h"

#include <Windows.h>

struct AContext {
	HWND window;
	HDC device;
	HGLRC context;
	HMODULE library;
	GLError error;
};
