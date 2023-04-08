#ifndef GL_CTX_TYPES
#define GL_CTX_TYPES
#include "api/ctx_api_types.h"

#include "utils/str.h"

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
typedef signed long int GLsizeiptr;
typedef signed long int GLintptr;

typedef struct GLError {
	GLint max_len;
	GLint max_count;
	GLenum *sources;
	GLenum *types;
	GLuint *ids;
	GLenum *severities;
	GLsizei *lengths;

	Str msgs;
	Str text;
	Str info;
} GLError;
#endif
