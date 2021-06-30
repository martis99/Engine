#pragma once
#ifdef E_OPENGL
#include "gl.h"
#include "api/gfx/afactor.h"

void gl_clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void gl_clear_depth();

void gl_cull_face_enable();
void gl_cull_face_disable();
void gl_cull_face_set_enabled(bool enabled);

void gl_depth_test_enable();
void gl_depth_test_disable();
void gl_depth_test_set_enabled(bool enabled);

void gl_polygon_mode_fill();
void gl_polygon_mode_line();

void gl_cull_face_back();
void gl_front_face_cw();

void gl_unpack_alignment();

void gl_blend_enable();
void gl_blend_disable();
void gl_blend_set_enabled(bool enabled);
void gl_blend_func(ASourceFactor sfactor, ADestinationFactor dfactor);

void gl_debug_output_enable();
void gl_debug_output_disable();
void gl_debug_output_set_enabled(bool enabled);
#endif