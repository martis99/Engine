#pragma once
#ifdef E_OPENGL
#include "gl.h"

GLuint gl_program_create();
void gl_program_attach_shader(GLuint program, GLuint shader);
void gl_program_link(GLuint program, GLint* status);
void gl_program_info_length(GLuint program, GLint* length);
void gl_program_info(GLuint program, GLsizei length, GLchar* info);
void gl_program_use(GLuint program);
GLint gl_program_get_uniform_location(GLuint program, const GLchar* name);
void gl_program_delete(GLuint program);
#endif