#include "pch.h"
#ifdef E_OPENGL
#include "gl_renderer.h"

void gl_clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(red, green, blue, alpha);
}
#endif