#pragma once
#ifdef GAPI_NONE
#include "api/atypes.h"

#include <Windows.h>

struct AWindow {
	byte id;
};

struct AContext {
	HWND window;
	HDC device;
};

struct ABuffer {
	byte id;
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
	byte id;
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
#endif