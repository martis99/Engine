#include "ctx_driver.h"

#include "ctx_gl_types.h"

#include "gl_ctx/gl_error.h"

#include "gl_ctx/c_glx.h"

#include <X11/Xlib.h>

static void *load_function(const char *name)
{
	void *p = glXGetProcAddress(name);
	if (p != 0) {
		return p;
	}
	return glXGetProcAddressARB(name);
}

#define LOAD_OPENGL_FUNCTION(name) (name = (name##Fn)load_function(#name))

static int load_functions()
{
	// clang-format off
	return
		LOAD_OPENGL_FUNCTION(glGetIntegerv) &&
		LOAD_OPENGL_FUNCTION(glClear) &&
		LOAD_OPENGL_FUNCTION(glClearColor) &&
		LOAD_OPENGL_FUNCTION(glClearDepth) &&
		LOAD_OPENGL_FUNCTION(glDrawArrays) &&
		LOAD_OPENGL_FUNCTION(glDrawElements) &&
		LOAD_OPENGL_FUNCTION(glPolygonMode) &&
		LOAD_OPENGL_FUNCTION(glEnable) &&
		LOAD_OPENGL_FUNCTION(glDisable) &&
		LOAD_OPENGL_FUNCTION(glCullFace) &&
		LOAD_OPENGL_FUNCTION(glFrontFace) &&
		LOAD_OPENGL_FUNCTION(glGenTextures) &&
		LOAD_OPENGL_FUNCTION(glBindTexture) &&
		LOAD_OPENGL_FUNCTION(glTexImage2D) &&
		LOAD_OPENGL_FUNCTION(glTexParameteri) &&
		LOAD_OPENGL_FUNCTION(glTexParameterf) &&
		LOAD_OPENGL_FUNCTION(glDeleteTextures) &&
		LOAD_OPENGL_FUNCTION(glBlendFunc) &&
		LOAD_OPENGL_FUNCTION(glDepthFunc) &&
		LOAD_OPENGL_FUNCTION(glPixelStorei) &&
		LOAD_OPENGL_FUNCTION(glBindTextureUnit) &&
		LOAD_OPENGL_FUNCTION(glViewport) &&

		LOAD_OPENGL_FUNCTION(glDebugMessageCallback) &&
		LOAD_OPENGL_FUNCTION(glDebugMessageControl) &&

		LOAD_OPENGL_FUNCTION(glGetString) &&
		LOAD_OPENGL_FUNCTION(glGetStringi) &&
		LOAD_OPENGL_FUNCTION(glGetError) &&
		LOAD_OPENGL_FUNCTION(glGetDebugMessageLog) &&

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
		LOAD_OPENGL_FUNCTION(glBindBufferBase) &&
		LOAD_OPENGL_FUNCTION(glBufferData) &&
		LOAD_OPENGL_FUNCTION(glBufferSubData) &&

		LOAD_OPENGL_FUNCTION(glEnableVertexAttribArray) &&
		LOAD_OPENGL_FUNCTION(glVertexAttribPointer) &&
		LOAD_OPENGL_FUNCTION(glVertexAttribIPointer) &&
		LOAD_OPENGL_FUNCTION(glVertexAttribLPointer) &&
		LOAD_OPENGL_FUNCTION(glVertexAttribDivisor) &&

		LOAD_OPENGL_FUNCTION(glGetUniformLocation) &&
		LOAD_OPENGL_FUNCTION(glGetUniformBlockIndex) &&
		LOAD_OPENGL_FUNCTION(glUniform1iv) &&
		LOAD_OPENGL_FUNCTION(glUniform2iv) &&
		LOAD_OPENGL_FUNCTION(glUniform3iv) &&
		LOAD_OPENGL_FUNCTION(glUniform4iv) &&
		LOAD_OPENGL_FUNCTION(glUniform1uiv) &&
		LOAD_OPENGL_FUNCTION(glUniform2uiv) &&
		LOAD_OPENGL_FUNCTION(glUniform3uiv) &&
		LOAD_OPENGL_FUNCTION(glUniform4uiv) &&
		LOAD_OPENGL_FUNCTION(glUniform1fv) &&
		LOAD_OPENGL_FUNCTION(glUniform2fv) &&
		LOAD_OPENGL_FUNCTION(glUniform3fv) &&
		LOAD_OPENGL_FUNCTION(glUniform4fv) &&
		LOAD_OPENGL_FUNCTION(glUniformMatrix4fv) &&
		LOAD_OPENGL_FUNCTION(glUniformBlockBinding) &&

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
		LOAD_OPENGL_FUNCTION(glClearBufferuiv) &&
		LOAD_OPENGL_FUNCTION(glClearBufferfv) &&
		LOAD_OPENGL_FUNCTION(glClearBufferfi) &&

		LOAD_OPENGL_FUNCTION(glGenRenderbuffers) &&
		LOAD_OPENGL_FUNCTION(glDeleteRenderbuffers) &&
		LOAD_OPENGL_FUNCTION(glBindRenderbuffer) &&
		LOAD_OPENGL_FUNCTION(glRenderbufferStorage) &&

		LOAD_OPENGL_FUNCTION(glGenerateMipmap) &&
		LOAD_OPENGL_FUNCTION(glActiveTexture) &&
		LOAD_OPENGL_FUNCTION(glBlendEquation) &&

		LOAD_OPENGL_FUNCTION(glDrawArraysInstanced) &&
		LOAD_OPENGL_FUNCTION(glDrawElementsInstanced);
	// clang-format on
}

typedef struct AWindow {
	Display *display;
	Window window;
} AWindow;

static AContext *ctx_create(void *window)
{
	AWindow *awindow  = window;
	AContext *context = m_malloc(sizeof(AContext));
	context->display  = awindow->display;
	context->window	  = awindow->window;

	if (load_functions() == 0) {
		printf("Failed to load opengl functions");
	}

	GLint att[]	= { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, None };
	XVisualInfo *vi = glXChooseVisual(context->display, 0, att);

	if (vi == NULL) {
		printf("Failed to choose visual\n");
		return NULL;
	}

	context->context = glXCreateContext(context->display, vi, NULL, GL_TRUE);
	glXMakeCurrent(context->display, context->window, context->context);

	if (gl_error_create(&context->error) == NULL) {
		return NULL;
	}

	GLint major;
	GLint minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("OpenGL version: %d.%d\n", major, minor);

	GLint extensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
	printf("%d extensions:\n", extensions);

	/*for(int i = 0; i < extensions; i++) {
		const char *extension = glGetStringi(GL_EXTENSIONS, i);
		printf("    %s\n", extension);
	}*/

	return context;
}

static void ctx_delete(AContext *context)
{
	gl_error_delete(&context->error);
	glXMakeCurrent(context->display, None, NULL);
	glXDestroyContext(context->display, context->context);
	m_free(context, sizeof(AContext));
}

static void ctx_swap_buffers(AContext *context)
{
	glXSwapBuffers(context->display, context->window);
}

static CtxDriver driver = {
	.ctx_create	  = ctx_create,
	.ctx_delete	  = ctx_delete,
	.ctx_swap_buffers = ctx_swap_buffers,
};

CTX_DRIVER(GL, 1, driver)
