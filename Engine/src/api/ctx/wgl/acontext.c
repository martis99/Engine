#include "pch.h"
#ifdef E_WINDOWS
#ifdef E_OPENGL
#include "api/ctx/acontext.h"
#include "api/wnd/awindow.h"

#include <Windows.h>

#include "api/gfx/opengl/gl/gl.h"
#include "wgl.h"

#pragma comment (lib, "opengl32.lib")
#pragma warning( disable : 6387 )

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
	HGLRC context;
	HMODULE library;
};

static PROC load_function(HMODULE library, const char* name) {
	PROC p = wglGetProcAddress(name);
	if (p != 0) {
		return p;
	}
	return GetProcAddress(library, name);
}

#define LOAD_OPENGL_FUNCTION(name) (name = (name##Fn)load_function(library, #name))

static int load_functions(HMODULE library) {
	return
		LOAD_OPENGL_FUNCTION(wglChoosePixelFormatARB) &&
		LOAD_OPENGL_FUNCTION(wglCreateContextAttribsARB) &&

		LOAD_OPENGL_FUNCTION(glDebugMessageCallback) &&
		LOAD_OPENGL_FUNCTION(glDebugMessageControl) &&

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
		LOAD_OPENGL_FUNCTION(glVertexAttribIPointer) &&

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

		LOAD_OPENGL_FUNCTION(glGenFramebuffers) &&
		LOAD_OPENGL_FUNCTION(glDeleteFramebuffers) &&
		LOAD_OPENGL_FUNCTION(glBindFramebuffer) &&
		LOAD_OPENGL_FUNCTION(glFramebufferTexture2D) &&
		LOAD_OPENGL_FUNCTION(glFramebufferRenderbuffer) &&
		LOAD_OPENGL_FUNCTION(glCheckFramebufferStatus) &&

		LOAD_OPENGL_FUNCTION(glReadBuffer) &&
		LOAD_OPENGL_FUNCTION(glReadPixels) &&

		LOAD_OPENGL_FUNCTION(glDrawBuffers) &&
		LOAD_OPENGL_FUNCTION(glClearBufferiv) &&
		LOAD_OPENGL_FUNCTION(glClearBufferfv) &&

		LOAD_OPENGL_FUNCTION(glGenRenderbuffers) &&
		LOAD_OPENGL_FUNCTION(glDeleteRenderbuffers) &&
		LOAD_OPENGL_FUNCTION(glBindRenderbuffer) &&
		LOAD_OPENGL_FUNCTION(glRenderbufferStorage) &&

		LOAD_OPENGL_FUNCTION(glGenerateMipmap) &&
		LOAD_OPENGL_FUNCTION(glActiveTexture) &&
		LOAD_OPENGL_FUNCTION(glBlendEquation);
}

static HMODULE load_opengl_functions(HMODULE module, LPCWSTR class_name) {
	HWND window = CreateWindowExW(WS_EX_APPWINDOW, class_name, L"", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, module, NULL);
	HDC device = GetDC(window);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
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

	int kFormatIndex = ChoosePixelFormat(device, &pfd);
	SetPixelFormat(device, kFormatIndex, &pfd);
	HGLRC context = wglCreateContext(device);
	wglMakeCurrent(device, context);

	HMODULE library = LoadLibraryA("OpenGL32.dll");
	load_functions(library);
	wglDeleteContext(context);
	DestroyWindow(window);
	return library;
}

static HGLRC create_context(HDC device) {
	{
		int attribs[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};

		int format;
		UINT num_formats;

		wglChoosePixelFormatARB(device, attribs, NULL, 1, &format, &num_formats);
		SetPixelFormat(device, format, 0);
	}
	{
		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			0
		};
		return wglCreateContextAttribsARB(device, NULL, attribs);
	}
}

AContext* acontext_create(AWindow* window) {
	AContext* context = m_malloc(sizeof(AContext));
	context->window = awindow_get_window(window);
	context->library = load_opengl_functions(window->module, window->class_name);
	context->device = GetDC(context->window);
	context->context = create_context(context->device);
	wglMakeCurrent(context->device, context->context);
	return context;
}

void acontext_delete(AContext* context) {
	FreeLibrary(context->library);
	ReleaseDC(context->window, context->device);
	wglDeleteContext(context->context);
	m_free(context, sizeof(AContext));
}

void acontext_swap_buffers(AContext* context) {
	wglSwapLayerBuffers(context->device, WGL_SWAP_MAIN_PLANE);
}
#endif
#endif