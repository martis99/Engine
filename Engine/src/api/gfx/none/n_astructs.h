#pragma once
#ifdef GAPI_NONE
#include "api/aenums.h"

#include <Windows.h>

struct AWindow {
	LPCWSTR class_name;
	HMODULE module;
	HWND window;
	AWindowCallbacks callbacks;
	ACursor* cursor;
};

struct AContext {
	HWND window;
	HDC device;
};

struct AFramebuffer {
	byte id;
};

struct AMesh {
	APrimitive primitive;
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
	char* name;
	ADataType type;
	void* data;
};

struct AUniformBuffer {
	byte id;
};
#endif