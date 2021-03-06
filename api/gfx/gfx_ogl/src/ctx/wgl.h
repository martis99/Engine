#pragma once
#include <Windows.h>

#define WGL_DRAW_TO_WINDOW_ARB        0x2001
#define WGL_SUPPORT_OPENGL_ARB        0x2010
#define WGL_DOUBLE_BUFFER_ARB         0x2011
#define WGL_PIXEL_TYPE_ARB            0x2013
#define WGL_COLOR_BITS_ARB            0x2014
#define WGL_DEPTH_BITS_ARB            0x2022
#define WGL_STENCIL_BITS_ARB          0x2023
#define WGL_TYPE_RGBA_ARB             0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB         0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB     0x00000001

#define DECL_WGL_FUNC(returnType, name, ...) typedef returnType(WINAPI *name##Fn)(__VA_ARGS__); name##Fn name;

DECL_WGL_FUNC(BOOL, wglChoosePixelFormatARB, HDC, const int*, const FLOAT*, UINT, int*, UINT*)
DECL_WGL_FUNC(HGLRC, wglCreateContextAttribsARB, HDC, HGLRC, const int*)