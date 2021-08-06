#pragma once
#ifdef GAPI_OPENGL
#include "gl.h"

GLuint gl_texture_create(GLint wrap, GLint filter, GLint width, GLint height, GLint internal_format, GLenum format, GLenum type, const GLvoid* data);
void gl_texture_bind(GLuint texture, GLuint slot);
void gl_texture_delete(GLuint texture);

#endif