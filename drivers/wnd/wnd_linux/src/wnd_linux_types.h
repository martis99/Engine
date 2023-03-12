#pragma once
#include "api/wnd_api_types.h"

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

struct ACursor {
	AWindow *window;
	bool enabled;
	bool in_window;
	LogCallbacks *log;
};

struct AWindow {
	Display *display;
	Window window;
	AWindowCallbacks callbacks;
	ACursor *cursor;
	LogCallbacks *log;
	Colormap cmap;
	XWindowAttributes att;
	XButtonEvent pointer;
};
