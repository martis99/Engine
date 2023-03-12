#include "api/wnd_api_cursor.h"
#include "api/wnd_api_window.h"

#include "wnd_linux_types.h"

#include "input/keys.h"

#include <X11/Xlib.h>

#include <string.h>

AWindow *awindow_create(AWindowCallbacks *callbacks, ACursor *cursor, int width, int height, LogCallbacks *log)
{
	AWindow *window	  = m_malloc(sizeof(AWindow));
	window->callbacks = *callbacks;
	window->cursor	  = cursor;
	window->log	  = log;

	window->display = XOpenDisplay(NULL);

	if (window->display == NULL) {
		printf("Cannot connect to X server\n");
		return NULL;
	}

	Window root = DefaultRootWindow(window->display);

	int screen = DefaultScreen(window->display);
	Visual *v  = DefaultVisual(window->display, screen);

	if (v == NULL) {
		printf("Failed to get default visual\n");
		return NULL;
	}

	int depth = DefaultDepth(window->display, screen);

	window->cmap = XCreateColormap(window->display, root, v, AllocNone);

	XSetWindowAttributes swa = { 0 };
	swa.colormap		 = window->cmap;
	swa.event_mask		 = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | PointerMotionHintMask | ResizeRedirectMask;

	window->window = XCreateWindow(window->display, root, 0, 0, width, height, 0, depth, InputOutput, v, CWColormap | CWEventMask, &swa);

	Atom wm_delete = XInternAtom(window->display, "WM_DELETE_WINDOW", 1);
	XSetWMProtocols(window->display, window->window, &wm_delete, 1);

	XMapWindow(window->display, window->window);

	XQueryPointer(window->display, window->window, &window->pointer.root, &window->pointer.subwindow, &window->pointer.x_root, &window->pointer.y_root,
		      &window->pointer.x, &window->pointer.y, &window->pointer.state);

	XGetWindowAttributes(window->display, window->window, &window->att);
	return window;
}

void awindow_delete(AWindow *window)
{
	XFreeColormap(window->display, window->cmap);
	XDestroyWindow(window->display, window->window);
	XCloseDisplay(window->display);
	m_free(window, sizeof(AWindow));
}

void *awindow_get_window(AWindow *window)
{
	return (void *)&window->window;
}

void awindow_set_title(AWindow *window, const char *title)
{
	XStoreName(window->display, window->window, title);
}

// clang-format off
unsigned int keys[136] = {
    [9] = K_ESCAPE,
    [10] = K_1,
    [11] = K_2,
    [12] = K_3,
    [13] = K_4,
    [14] = K_5,
    [15] = K_6,
    [16] = K_7,
    [17] = K_8,
    [18] = K_9,
    [19] = K_0,
    [20] = K_UNDERSCORE,
    [21] = K_EQUAL,
    [22] = K_BACKSPACE,
    [23] = K_TAB,
    [24] = K_Q,
    [25] = K_W,
    [26] = K_E,
    [27] = K_R,
    [28] = K_T,
    [29] = K_Y,
    [30] = K_U,
    [31] = K_I,
    [32] = K_O,
    [33] = K_P,
    [34] = K_BRACKETLEFT,
    [35] = K_BRACKETRIGHT,
    [36] = K_RETURN,
    [37] = K_LCONTROL,
    [38] = K_A,
    [39] = K_S,
    [40] = K_D,
    [41] = K_F,
    [42] = K_G,
    [43] = K_H,
    [44] = K_J,
    [45] = K_K,
    [46] = K_L,
    [47] = K_SEMICOLON,
    [48] = K_QUOTE,
    [49] = K_BACKQUOTE,
    [50] = K_LSHIFT,
    [51] = K_BACKSLASH,
    [52] = K_Z,
    [53] = K_X,
    [54] = K_C,
    [55] = K_V,
    [56] = K_B,
    [57] = K_N,
    [58] = K_M,
    [59] = K_COMMA,
    [60] = K_PUNT,
    [61] = K_QUESTION,
    [62] = K_RSHIFT,
    [63] = K_MULTIPLY,
    [64] = K_ALT,
    [65] = K_SPACE,
    [66] = K_CAPSLOCK,
    [67] = K_F1,
    [68] = K_F2,
    [69] = K_F3,
    [70] = K_F4,
    [71] = K_F5,
    [72] = K_F6,
    [73] = K_F7,
    [74] = K_F8,
    [75] = K_F9,
    [76] = K_F10,
    [77] = K_NUMLOCK,
    [78] = K_SCROLLLOCK,
    [79] = K_NUMPAD7,
    [80] = K_NUMPAD8,
    [81] = K_NUMPAD9,
    [82] = K_MINUS,
    [83] = K_NUMPAD4,
    [84] = K_NUMPAD5,
    [85] = K_NUMPAD6,
    [86] = K_ADD,
    [87] = K_NUMPAD1,
    [88] = K_NUMPAD2,
    [89] = K_NUMPAD3,
    [90] = K_NUMPAD0,
    [91] = K_NUMPADDEL,
    [95] = K_F11,
    [96] = K_F12,
    [104] = K_NUMPADRETURN,
    [105] = K_RCONTROL,
    [106] = K_SLASH,
    [107] = K_PRINT,
    [108] = K_ALTGR,
    [110] = K_HOME,
    [111] = K_ARROWUP,
    [112] = K_PAGEUP,
    [113] = K_ARROWLEFT,
    [114] = K_ARROWRIGHT,
    [115] = K_END,
    [116] = K_ARROWDOWN,
    [117] = K_PAGEDOWN,
    [118] = K_INSERT,
    [119] = K_DELETE,
    [127] = K_PAUSE,
    [133] = K_LOS,
    [134] = K_ROS,
    [135] = K_CONTEXTMENU,
};
// clang-format on

unsigned int mouse[10] = {
	[1] = K_MOUSEL, [2] = K_MOUSEM, [3] = K_MOUSER, [4] = K_MOUSEMUP, [5] = K_MOUSEMDOWN, [8] = K_MOUSES0, [9] = K_MOUSES1,
};

int awindow_poll_events(AWindow *window)
{
	XButtonEvent pointer;
	XQueryPointer(window->display, window->window, &pointer.root, &pointer.subwindow, &pointer.x_root, &pointer.y_root, &pointer.x, &pointer.y, &pointer.state);

	if (window->pointer.x != pointer.x || window->pointer.y != pointer.y) {
		if (window->pointer.x > 0 && window->pointer.y > 0 && window->pointer.x < window->att.width && window->pointer.y < window->att.height) {
			window->callbacks.mouse_moved(window->callbacks.priv, (float)pointer.x, (float)pointer.y);
			window->callbacks.mouse_moved_delta(window->callbacks.priv, (float)window->pointer.x - pointer.x, (float)window->pointer.y - pointer.y);
		}
		window->pointer = pointer;
	}

	XEvent xev;
	while (XPending(window->display)) {
		XNextEvent(window->display, &xev);

		if (XFilterEvent(&xev, window->window)) {
			continue;
		}

		switch (xev.type) {
		case KeyPress: window->callbacks.key_pressed(window->callbacks.priv, keys[xev.xkey.keycode]); break;
		case KeyRelease: window->callbacks.key_released(window->callbacks.priv, keys[xev.xkey.keycode]); break;
		case ButtonPress: {
			window->callbacks.mouse_pressed(window->callbacks.priv, mouse[xev.xkey.keycode]);
			switch (xev.xkey.keycode) {
			case 4: window->callbacks.mouse_wheel(window->callbacks.priv, -40); break;
			case 5: window->callbacks.mouse_wheel(window->callbacks.priv, 40); break;
			}
			break;
		}
		case ButtonRelease: window->callbacks.mouse_released(window->callbacks.priv, mouse[xev.xkey.keycode]); break;
		case Expose: XGetWindowAttributes(window->display, window->window, &window->att); break;
		case ClientMessage: {
			if (!strcmp(XGetAtomName(window->display, xev.xclient.message_type), "WM_PROTOCOLS")) {
				return 0;
			}
			break;
		}
		}
	}

	return 1;
}

void awindow_message_box(AWindow *window, const char *text, const char *caption)
{
}

void awindow_message_boxw(AWindow *window, const wchar *text, const wchar *caption)
{
}

void awindow_close(AWindow *window)
{
}
