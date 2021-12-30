#pragma once
#include "api/gfx/gfx_api_types.h"

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

typedef char GLchar;
typedef unsigned int GLsizeiptr;
typedef unsigned int GLintptr;

typedef struct GLError {
	GLint max_len;
	GLint max_count;
	GLenum* sources;
	GLenum* types;
	GLuint* ids;
	GLenum* severities;
	GLsizei* lengths;

	Str msgs;
	Str text;
	Str info;

	AErrorCallbacks callbacks;
} GLError;