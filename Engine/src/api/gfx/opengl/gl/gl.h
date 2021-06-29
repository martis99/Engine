#pragma once
#ifdef E_OPENGL
#ifdef E_WINDOWS
#include "gl_defines.h"

#include <Windows.h>
#endif

#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LONG_LENGTH 0x8B84
#define GL_COMPILE_STATUS 0x8B81
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_FLOAT 0x1406

#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242

#define GL_R8 0x8229

#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_CLAMP_TO_EDGE 0x812F

#define GL_TEXTURE0 0x84C0

#define GL_FUNC_ADD 0x8006
#define GL_MAX 0x8008

#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251

#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148

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

#define DECL_OPENGL_FUNC(returnType, name, ...) typedef returnType(WINAPI *name##Fn)(__VA_ARGS__); name##Fn name;

DECL_OPENGL_FUNC(GLuint, glCreateProgram, void)
DECL_OPENGL_FUNC(void, glDeleteProgram, GLuint program)
DECL_OPENGL_FUNC(void, glLinkProgram, GLuint program)
DECL_OPENGL_FUNC(void, glGetProgramiv, GLuint program, GLenum pname, GLint* params)
DECL_OPENGL_FUNC(void, glGetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog)
DECL_OPENGL_FUNC(void, glUseProgram, GLuint program)

DECL_OPENGL_FUNC(GLuint, glCreateShader, GLenum shaderType)
DECL_OPENGL_FUNC(void, glDeleteShader, GLuint shader)
DECL_OPENGL_FUNC(void, glCompileShader, GLuint shader)
DECL_OPENGL_FUNC(void, glAttachShader, GLuint program, GLuint shader)
DECL_OPENGL_FUNC(void, glGetShaderiv, GLuint shader, GLenum pname, GLint* params)
DECL_OPENGL_FUNC(void, glGetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog)
DECL_OPENGL_FUNC(void, glShaderSource, GLuint shader, GLsizei count, const GLchar** string, GLint* length)

DECL_OPENGL_FUNC(void, glGenVertexArrays, GLsizei n, GLuint* arrays)
DECL_OPENGL_FUNC(void, glCreateVertexArrays, GLsizei n, GLuint* arrays)
DECL_OPENGL_FUNC(void, glDeleteVertexArrays, GLsizei n, GLuint* arrays)
DECL_OPENGL_FUNC(void, glBindVertexArray, GLuint array)

DECL_OPENGL_FUNC(void, glDeleteBuffers, GLsizei n, GLuint* buffers)
DECL_OPENGL_FUNC(void, glBindBuffer, GLenum target, GLuint buffer)
DECL_OPENGL_FUNC(void, glBufferData, GLenum target, GLsizeiptr size, const void* data, GLenum usage)
DECL_OPENGL_FUNC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
DECL_OPENGL_FUNC(void, glGenBuffers, GLsizei n, GLuint* buffers)

DECL_OPENGL_FUNC(void, glEnableVertexAttribArray, GLuint index)
DECL_OPENGL_FUNC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)

DECL_OPENGL_FUNC(GLint, glGetUniformLocation, GLuint program, const GLchar* name)
DECL_OPENGL_FUNC(void, glUniform1iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform2iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform3iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform4iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform1fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniform2fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniform3fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniform4fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)

DECL_OPENGL_FUNC(void, glGenerateMipmap, GLenum target)
DECL_OPENGL_FUNC(void, glActiveTexture, GLenum texture)
DECL_OPENGL_FUNC(void, glBlendEquation, GLenum mode)

typedef void(*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
DECL_OPENGL_FUNC(void, glDebugMessageCallback, DEBUGPROC callback, const void* userParam)
DECL_OPENGL_FUNC(void, glDebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled)

void WINAPI glClear(GLbitfield mask);
void WINAPI glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void WINAPI glDrawArrays(GLenum mode, GLint first, GLsizei count);
void WINAPI glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
void WINAPI glPolygonMode(GLenum face, GLenum mode);
void WINAPI glEnable(GLenum cap);
void WINAPI glDisable(GLenum cap);
void WINAPI glCullFace(GLenum mode);
void WINAPI glFrontFace(GLenum mode);
void WINAPI glGenTextures(GLsizei n, GLuint* textures);
void WINAPI glBindTexture(GLenum target, GLuint texture);
void WINAPI glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);
void WINAPI glTexParameteri(GLenum target, GLenum pname, GLint param);
void WINAPI glDeleteTextures(GLsizei n, const GLuint* textures);
void WINAPI glBlendFunc(GLenum sfactor, GLenum dfactor);
void WINAPI glPixelStorei(GLenum pname, GLint param);
void WINAPI glBindTextureUnit(GLuint unit, GLuint texture);

int gl_load_functions(HMODULE library);
#endif