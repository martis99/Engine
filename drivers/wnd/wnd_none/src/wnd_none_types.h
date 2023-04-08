#pragma once
#include "api/wnd_api_types.h"

struct ACursor {
	AWindow *window;
	bool enabled;
	bool in_window;
};

struct AWindow {
	AWindowCallbacks callbacks;
	ACursor *cursor;
};
