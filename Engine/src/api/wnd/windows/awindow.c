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
	AWindowCallbacks callbacks;
};

static LRESULT CALLBACK wnd_proc(HWND wnd, UINT message, WPARAM param_w, LPARAM param_l) {

	if (message == WM_CREATE) {
		CREATESTRUCT* create = (CREATESTRUCT*)param_l;
		AWindow* data = (AWindow*)(create->lpCreateParams);
		SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)data);
	}

	AWindow* window = (AWindow*)GetWindowLongPtr(wnd, GWLP_USERDATA);
	if (window == NULL) {
		return DefWindowProc(wnd, message, param_w, param_l);
	}

	switch (message) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		window->callbacks.key_pressed((byte)param_w);
		break;
	case WM_KEYUP:
		window->callbacks.key_released((byte)param_w);
		break;
	}
	return DefWindowProc(wnd, message, param_w, param_l);
}

AWindow* awindow_create(AWindowCallbacks* callbacks, int width, int height) {
	AWindow* window = m_malloc(sizeof(AWindow));
	window->instance = GetModuleHandleW(0);
	window->callbacks = *callbacks;

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = wnd_proc;
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

void awindow_set_title(AWindow* window, const char* title) {
	if (SetWindowTextA(window->wnd, title) == 0) {
		log_error("Failed to set title");
	}
}

int awindow_poll_events(AWindow* window) {
	MSG msg = { 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return 0;
		}
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 1;
}

void awindow_close(AWindow* window) {
	PostQuitMessage(0);
}
#endif