#pragma once
#ifdef GAPI_OPENGL
#include "gl.h"

void gl_clear_buffers();
void gl_clear_buffer_color();
void gl_clear_buffer_depth();
void gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

void gl_cull_face_enable();
void gl_cull_face_disable();

void gl_depth_test_enable();
void gl_depth_test_disable();

void gl_polygon_mode_fill();
void gl_polygon_mode_line();

void gl_cull_face_back();
void gl_front_face_cw();

void gl_unpack_alignment();

void gl_blend_enable();
void gl_blend_disable();
void gl_blend_func(GLenum sfactor, GLenum dfactor);

void gl_debug_output_enable();
void gl_debug_output_disable();
#endif