#pragma once
#ifdef E_OPENGL
#include "gl.h"

GLuint gl_texture_create(GLint wrap, GLint filter);
GLuint gl_texture_generate();
void gl_texture_bind(GLuint texture, GLuint slot);
void gl_texture_generate_mipmap();
void gl_texture_image2d(GLuint texture, GLint width, GLint height, GLint channels, const GLvoid* data);
void gl_texture_delete(GLuint texture);
#endif