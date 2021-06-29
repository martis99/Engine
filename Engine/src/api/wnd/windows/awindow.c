#include "pch.h"
#ifdef E_WINDOWS
#include "api/wnd/awindow.h"

#include <Windows.h>

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")

static const LPCWSTR s_ClassName = L"EngineClass";
static const LPCWSTR s_WindowName = L"Engine";
static const DWORD s_ExStyle = 0;
static const DWORD s_Style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

struct AWindow {
	HINSTANCE instance;
	HWND wnd;
};

AWindow* awindow_create(int width, int height) {
	AWindow* window = m_malloc(sizeof(AWindow));
	window->instance = GetModuleHandleW(0);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = window->instance;
	wc.lpszClassName = s_ClassName;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc)) {
		log_error("Failed to register class");
		return NULL;
	}

	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, s_Style, FALSE, s_ExStyle);

	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);

	window->wnd = CreateWindowExW(
		s_ExStyle,
		wc.lpszClassName,
		s_WindowName,
		s_Style,
		(screen_w - (rect.right - rect.left)) / 2,
		(screen_h - height) / 2 + rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		window->instance,
		window
	);

	ShowWindow(window->wnd, SW_SHOW);
	return window;
}

void awindow_delete(AWindow* window) {
	DestroyWindow(window->wnd);
	UnregisterClassW(s_ClassName, window->instance);
	m_free(window, sizeof(AWindow));
}
#endif