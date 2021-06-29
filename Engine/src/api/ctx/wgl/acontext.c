#include "pch.h"
#ifdef E_WINDOWS
#ifdef E_OPENGL
#include "api/ctx/acontext.h"
#include "api/wnd/awindow.h"

#include <Windows.h>

#include "api/gfx/opengl/gl/gl.h"

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

#define LOAD_OPENGL_FUNCTION(name) (name = (name##Fn)load_opengl_func(library, #name))

static PROC load_opengl_func(HMODULE library, const char* name) {
	PROC p = wglGetProcAddress(name);
	if (p != 0) {
		return p;
	}
	return GetProcAddress(library, name);
}

static int load_functions(HMODULE library) {
	return
		LOAD_OPENGL_FUNCTION(glCreateProgram) &&
		LOAD_OPENGL_FUNCTION(glDeleteProgram) &&
		LOAD_OPENGL_FUNCTION(glLinkProgram) &&
		LOAD_OPENGL_FUNCTION(glGetProgramiv) &&
		LOAD_OPENGL_FUNCTION(glGetProgramInfoLog) &&
		LOAD_OPENGL_FUNCTION(glUseProgram) &&

		LOAD_OPENGL_FUNCTION(glCreateShader) &&
		LOAD_OPENGL_FUNCTION(glDeleteShader) &&
		LOAD_OPENGL_FUNCTION(glCompileShader) &&
		LOAD_OPENGL_FUNCTION(glAttachShader) &&
		LOAD_OPENGL_FUNCTION(glGetShaderiv) &&
		LOAD_OPENGL_FUNCTION(glGetShaderInfoLog) &&
		LOAD_OPENGL_FUNCTION(glShaderSource) &&

		LOAD_OPENGL_FUNCTION(glGenVertexArrays) &&
		LOAD_OPENGL_FUNCTION(glDeleteVertexArrays) &&
		LOAD_OPENGL_FUNCTION(glBindVertexArray) &&

		LOAD_OPENGL_FUNCTION(glGenBuffers) &&
		LOAD_OPENGL_FUNCTION(glDeleteBuffers) &&
		LOAD_OPENGL_FUNCTION(glBindBuffer) &&
		LOAD_OPENGL_FUNCTION(glBufferData) &&
		LOAD_OPENGL_FUNCTION(glBufferSubData) &&

		LOAD_OPENGL_FUNCTION(glEnableVertexAttribArray) &&
		LOAD_OPENGL_FUNCTION(glVertexAttribPointer) &&

		LOAD_OPENGL_FUNCTION(glGetUniformLocation) &&
		LOAD_OPENGL_FUNCTION(glUniform1iv) &&
		LOAD_OPENGL_FUNCTION(glUniform2iv) &&
		LOAD_OPENGL_FUNCTION(glUniform3iv) &&
		LOAD_OPENGL_FUNCTION(glUniform4iv) &&
		LOAD_OPENGL_FUNCTION(glUniform1fv) &&
		LOAD_OPENGL_FUNCTION(glUniform2fv) &&
		LOAD_OPENGL_FUNCTION(glUniform3fv) &&
		LOAD_OPENGL_FUNCTION(glUniform4fv) &&
		LOAD_OPENGL_FUNCTION(glUniformMatrix4fv) &&

		LOAD_OPENGL_FUNCTION(glDebugMessageCallback) &&
		LOAD_OPENGL_FUNCTION(glDebugMessageControl) &&

		LOAD_OPENGL_FUNCTION(glGenerateMipmap) &&
		LOAD_OPENGL_FUNCTION(glActiveTexture) &&

		LOAD_OPENGL_FUNCTION(glBlendEquation);
}

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

	context->library = LoadLibraryA("opengl32.dll");
	if (context->library == 0) {
		log_error("Failed to initialize OpenGL");
		return NULL;
	}

	if (load_functions(context->library) == 0) {
		log_error("Failed to load OpenGL functions");
		return NULL;
	}

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