#pragma once
#include "gl_defines.h"
#include "gl_types.h"

#include <Windows.h>

#define DECL_OPENGL_FUNC(returnType, name, ...) typedef returnType(WINAPI *name##Fn)(__VA_ARGS__); name##Fn name;

typedef void(*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
DECL_OPENGL_FUNC(void, glDebugMessageCallback, DEBUGPROC callback, const void* userParam)
DECL_OPENGL_FUNC(void, glDebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled)

DECL_OPENGL_FUNC(const GLubyte*, glGetString, GLenum name)
DECL_OPENGL_FUNC(const GLubyte*, glGetStringi, GLenum name, GLuint index)
DECL_OPENGL_FUNC(GLenum, glGetError, void)
DECL_OPENGL_FUNC(GLuint, glGetDebugMessageLog, GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog)

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
DECL_OPENGL_FUNC(void, glDeleteVertexArrays, GLsizei n, const GLuint* arrays)
DECL_OPENGL_FUNC(void, glBindVertexArray, GLuint array)

DECL_OPENGL_FUNC(void, glGenBuffers, GLsizei n, GLuint* buffers)
DECL_OPENGL_FUNC(void, glDeleteBuffers, GLsizei n, const GLuint* buffers)
DECL_OPENGL_FUNC(void, glBindBuffer, GLenum target, GLuint buffer)
DECL_OPENGL_FUNC(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer)
DECL_OPENGL_FUNC(void, glBufferData, GLenum target, GLsizeiptr size, const void* data, GLenum usage)
DECL_OPENGL_FUNC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void* data)

DECL_OPENGL_FUNC(void, glEnableVertexAttribArray, GLuint index)
DECL_OPENGL_FUNC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
DECL_OPENGL_FUNC(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
DECL_OPENGL_FUNC(void, glVertexAttribLPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
DECL_OPENGL_FUNC(void, glVertexAttribDivisor, GLuint index, GLuint divisor)

DECL_OPENGL_FUNC(GLint, glGetUniformLocation, GLuint program, const GLchar* name)
DECL_OPENGL_FUNC(GLint, glGetUniformBlockIndex, GLuint program, const GLchar* uniformBlockName)
DECL_OPENGL_FUNC(void, glUniform1iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform2iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform3iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform4iv, GLint location, GLsizei count, const GLint* value)
DECL_OPENGL_FUNC(void, glUniform1uiv, GLint location, GLsizei count, const GLuint* value)
DECL_OPENGL_FUNC(void, glUniform2uiv, GLint location, GLsizei count, const GLuint* value)
DECL_OPENGL_FUNC(void, glUniform3uiv, GLint location, GLsizei count, const GLuint* value)
DECL_OPENGL_FUNC(void, glUniform4uiv, GLint location, GLsizei count, const GLuint* value)
DECL_OPENGL_FUNC(void, glUniform1fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniform2fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniform3fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniform4fv, GLint location, GLsizei count, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
DECL_OPENGL_FUNC(void, glUniformBlockBinding, GLuint program, GLuint uniformBlockIndexm, GLuint uniformBlockBinding)

DECL_OPENGL_FUNC(void, glGenFramebuffers, GLsizei n, GLuint* framebuffers)
DECL_OPENGL_FUNC(void, glDeleteFramebuffers, GLsizei n, GLuint* framebuffers)
DECL_OPENGL_FUNC(void, glBindFramebuffer, GLenum target, GLuint framebuffer)
DECL_OPENGL_FUNC(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
DECL_OPENGL_FUNC(GLenum, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
DECL_OPENGL_FUNC(GLenum, glCheckFramebufferStatus, GLenum target)

DECL_OPENGL_FUNC(void, glReadBuffer, GLenum mode)
DECL_OPENGL_FUNC(void, glReadPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data)

DECL_OPENGL_FUNC(void, glDrawBuffers, GLsizei n, const GLenum* bufs)
DECL_OPENGL_FUNC(void, glClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint* value)
DECL_OPENGL_FUNC(void, glClearBufferuiv, GLenum buffer, GLint drawbuffer, const GLuint* value)
DECL_OPENGL_FUNC(void, glClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat* value)
DECL_OPENGL_FUNC(void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)

DECL_OPENGL_FUNC(void, glGenRenderbuffers, GLsizei n, GLuint* renderbufers)
DECL_OPENGL_FUNC(void, glDeleteRenderbuffers, GLsizei n, GLuint* renderbuffers)
DECL_OPENGL_FUNC(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer)
DECL_OPENGL_FUNC(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height)

DECL_OPENGL_FUNC(void, glGenerateMipmap, GLenum target)
DECL_OPENGL_FUNC(void, glActiveTexture, GLenum texture)
DECL_OPENGL_FUNC(void, glBlendEquation, GLenum mode)

DECL_OPENGL_FUNC(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
DECL_OPENGL_FUNC(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instancecount)

void WINAPI glGetIntegerv(GLenum pname, GLint* data);
void WINAPI glClear(GLbitfield mask);
void WINAPI glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void WINAPI glClearDepth(GLdouble depth);
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
void WINAPI glTexParameterf(GLenum target, GLenum pname, GLfloat param);
void WINAPI glDeleteTextures(GLsizei n, const GLuint* textures);
void WINAPI glBlendFunc(GLenum sfactor, GLenum dfactor);
void WINAPI glPixelStorei(GLenum pname, GLint param);
void WINAPI glBindTextureUnit(GLuint unit, GLuint texture);