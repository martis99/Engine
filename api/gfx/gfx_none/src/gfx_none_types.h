#pragma once
#include "api/gfx/gfx_api_types.h"

#include <Windows.h>

struct AContext {
	HWND window;
	HDC device;
};

struct AMaterial {
	byte id;
};

struct AFramebuffer {
	byte id;
};

struct AMesh {
	byte id;
};

struct ARenderer {
	int lhc;
};

struct AShader {
	byte id;
};

struct ATexture {
	byte id;
};

struct AUniform {
	byte id;
};

struct AUniformBuffer {
	byte id;
};