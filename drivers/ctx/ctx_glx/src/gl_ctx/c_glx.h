#pragma once
#include <X11/Xlib.h>

#include "gl_ctx_types.h"

#define GLX_USE_GL	     1
#define GLX_BUFFER_SIZE	     2
#define GLX_LEVEL	     3
#define GLX_RGBA	     4
#define GLX_DOUBLEBUFFER     5
#define GLX_STEREO	     6
#define GLX_AUX_BUFFERS	     7
#define GLX_RED_SIZE	     8
#define GLX_GREEN_SIZE	     9
#define GLX_BLUE_SIZE	     10
#define GLX_ALPHA_SIZE	     11
#define GLX_DEPTH_SIZE	     12
#define GLX_STENCIL_SIZE     13
#define GLX_ACCUM_RED_SIZE   14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE  16
#define GLX_ACCUM_ALPHA_SIZE 17

typedef struct {
	Visual *visual;
	VisualID visualid;
	int screen;
	int depth;
	int class;
	unsigned long red_mask;
	unsigned long green_mask;
	unsigned long blue_mask;
	int colormap_size;
	int bits_per_rgb;
} XVisualInfo;

typedef XID GLXDrawable;
typedef struct __GLXcontextRec *GLXContext;

#define DECL_GLX_FUNC(returnType, name, ...) extern returnType name(__VA_ARGS__);

DECL_GLX_FUNC(XVisualInfo *, glXChooseVisual, Display *dpy, int screen, int *attribList);
DECL_GLX_FUNC(GLXContext, glXCreateContext, Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
DECL_GLX_FUNC(void, glXDestroyContext, Display *dpy, GLXContext ctx);
DECL_GLX_FUNC(Bool, glXMakeCurrent, Display *dpy, GLXDrawable drawable, GLXContext ctx);
DECL_GLX_FUNC(void, glXSwapBuffers, Display *dpy, GLXDrawable drawable);

extern void (*glXGetProcAddress(const GLubyte *procname))(void);
typedef void (*__GLXextFuncPtr)(void);
extern __GLXextFuncPtr glXGetProcAddressARB(const GLubyte *);
