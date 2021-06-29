#include "pch.h"
#ifdef E_WINDOWS
#ifdef E_OPENGL
#include "api/ctx/acontext.h"
#include "api/wnd/awindow.h"

#include <Windows.h>

#pragma comment (lib, "opengl32.lib")

struct AWindow {
	HINSTANCE instance;
	HWND wnd;
	AWindowCallbacks callbacks;
	ACursor* cursor;
};

struct AContext {
	AWindow* window;
	HDC device;
	HGLRC rendering;
	HMODULE library;
};

AContext* acontext_create(AWindow* window) {
	AContext* context = m_malloc(sizeof(AContext));
	context->window = window;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		3,
		PFD_DRAW_TO_WINDOW,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	context->device = GetDC(window->wnd);
	int pixelFormat = ChoosePixelFormat(context->device, &pfd);
	SetPixelFormat(context->device, pixelFormat, &pfd);

	context->rendering = wglCreateContext(context->device);
	wglMakeCurrent(context->device, context->rendering);

	return context;
}

void acontext_delete(AContext* context) {
	ReleaseDC(context->window->wnd, context->device);
	wglDeleteContext(context->rendering);
	m_free(context, sizeof(AContext));
}

void acontext_swap_buffers(AContext* context) {
	wglSwapLayerBuffers(context->device, WGL_SWAP_MAIN_PLANE);
}
#endif
#endif