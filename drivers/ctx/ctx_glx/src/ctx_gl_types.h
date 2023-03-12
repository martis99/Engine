#pragma once

#include "gl_ctx/gl_ctx_types.h"

#include "gl_ctx/c_glx.h"

struct AContext {
	Display *display;
	Window window;
	GLXContext context;
	GLError error;
};
