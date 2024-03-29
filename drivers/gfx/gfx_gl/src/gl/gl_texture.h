#pragma once
#include "gl_types.h"

GLuint gl_texture_create(GLError *error, GLint wrap, AFilter filter, GLint width, GLint height, GLint internal_format, GLenum format, GLenum type, const GLvoid *data);
void gl_texture_bind(GLError *error, GLuint texture, GLuint slot);
void gl_texture_delete(GLError *error, GLuint texture);
