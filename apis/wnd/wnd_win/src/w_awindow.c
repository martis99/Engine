#include "api/awindow.h"
#include "api/acursor.h"

#include  "wnd_win_types.h"

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")

static const LPCWSTR s_ClassName = L"EngineClass";
static const LPCWSTR s_WindowName = L"Engine";
static const DWORD s_ExStyle = 0;
static const DWORD s_Style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

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

	ACursor* cursor = window->cursor;
	byte cursor_enbled = acursor_get_enabled(window->cursor);
	byte cursor_in_window = acursor_get_in_window(window->cursor);

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
	case WM_ACTIVATE:
	{
		if (cursor_enbled == 0) {
			if (param_w & WA_ACTIVE) {
				acursor_confine(cursor);
				acursor_hide(cursor);
			} else {
				acursor_free(cursor);
				acursor_show(cursor);
			}
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(param_l);
		if (cursor_enbled == 0) {
			if (cursor_in_window == 0) {
				SetCapture(wnd);
				acursor_set_in_window(cursor, 1);
				acursor_hide(cursor);
			}
			break;
		}
		if (pt.x >= 0 && pt.x < 1600 && pt.y >= 0 && pt.y < 900) {
			window->callbacks.mouse_moved((float)pt.x, (float)pt.y);
			if (cursor_in_window == 0) {
				SetCapture(wnd);
				acursor_set_in_window(cursor, 1);
			}
		} else {
			if (param_w & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON)) {
				window->callbacks.mouse_moved((float)pt.x, (float)pt.y);
			} else {
				ReleaseCapture();
				acursor_set_in_window(cursor, 0);
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(wnd);
		if (cursor_enbled == 0) {
			acursor_confine(cursor);
			acursor_hide(cursor);
		}
		window->callbacks.mouse_pressed(0);
		break;
	}
	case WM_RBUTTONDOWN:
		SetForegroundWindow(wnd);
		if (cursor_enbled == 0) {
			acursor_confine(cursor);
			acursor_hide(cursor);
		}
		window->callbacks.mouse_pressed(1);
		break;
	case WM_MBUTTONDOWN:
		SetForegroundWindow(wnd);
		if (cursor_enbled == 0) {
			acursor_confine(cursor);
			acursor_hide(cursor);
		}
		window->callbacks.mouse_pressed(2);
		break;

	case WM_LBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(param_l);
		if (pt.x < 0 || pt.x >= 1600 || pt.y < 0 || pt.y >= 900) {
			ReleaseCapture();
			acursor_set_in_window(cursor, 0);
		}
		window->callbacks.mouse_released(0);
		break;
	}

	case WM_RBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(param_l);
		if (pt.x < 0 || pt.x >= 1600 || pt.y < 0 || pt.y >= 900) {
			ReleaseCapture();
			acursor_set_in_window(cursor, 0);
		}
		window->callbacks.mouse_released(1);
		break;
	}
	case WM_MBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(param_l);
		if (pt.x < 0 || pt.x >= 1600 || pt.y < 0 || pt.y >= 900) {
			ReleaseCapture();
			acursor_set_in_window(cursor, 0);
		}
		window->callbacks.mouse_released(2);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		short delta = GET_WHEEL_DELTA_WPARAM(param_w);
		window->callbacks.mouse_wheel((float)delta);
		break;
	}
	case WM_INPUT:
	{
		UINT size = 0;
		if (GetRawInputData((HRAWINPUT)param_l, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER)) == (UINT)-1) {
			break;
		}
		BYTE* data = (BYTE*)m_malloc(size);
		if (GetRawInputData((HRAWINPUT)param_l, RID_INPUT, data, &size, sizeof(RAWINPUTHEADER)) != size) {
			break;
		}
		RAWINPUT* ri = (RAWINPUT*)data;
		if (ri->header.dwType == RIM_TYPEMOUSE && (ri->data.mouse.lLastX != 0 || ri->data.mouse.lLastY != 0)) {
			window->callbacks.mouse_moved_delta((float)ri->data.mouse.lLastX, (float)ri->data.mouse.lLastY);
		}
		m_free(data, size);
		break;
	}
	}
	return DefWindowProc(wnd, message, param_w, param_l);
}

AWindow* awindow_create(AWindowCallbacks* callbacks, ACursor* cursor, int width, int height, LogCallbacks* log) {
	AWindow* window = m_malloc(sizeof(AWindow));
	window->class_name = s_ClassName;
	window->module = GetModuleHandleW(0);
	window->callbacks = *callbacks;
	window->cursor = cursor;
	window->log = log;

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = wnd_proc;
	wc.hInstance = window->module;
	wc.lpszClassName = s_ClassName;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc)) {
		log_msg(window->log, "Failed to register class");
		return NULL;
	}

	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, s_Style, FALSE, s_ExStyle);

	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);

	window->window = CreateWindowExW(
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
		window->module,
		window
	);

	ShowWindow(window->window, SW_SHOW);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = NULL;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) {
		log_msg(window->log, "Failed to register raw input device");
		return NULL;
	}

	return window;
}

void awindow_delete(AWindow* window) {
	DestroyWindow(window->window);
	UnregisterClassW(s_ClassName, window->module);
	m_free(window, sizeof(AWindow));
}

void* awindow_get_window(AWindow* window) {
	return window->window;
}

void awindow_set_title(AWindow* window, const char* title) {
	if (SetWindowTextA(window->window, title) == 0) {
		log_msg(window->log, "Failed to set title");
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

void awindow_message_box(AWindow* window, const char* text, const char* caption) {
	MessageBoxA(window->window, text, caption, MB_ICONERROR);
}

void awindow_message_boxw(AWindow* window, const wchar* text, const wchar* caption) {
	MessageBoxW(window->window, text, caption, MB_ICONERROR);
}

void awindow_close(AWindow* window) {
	PostQuitMessage(0);
}