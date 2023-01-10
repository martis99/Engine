#pragma once
#include "gl_types.h"

void gl_cull_face_enable(GLError *error);
void gl_cull_face_disable(GLError *error);

void gl_depth_test_enable(GLError *error);
void gl_depth_test_disable(GLError *error);

void gl_polygon_mode_fill(GLError *error);
void gl_polygon_mode_line(GLError *error);

void gl_cull_face_back(GLError *error);
void gl_front_face_cw(GLError *error);
void gl_front_face_ccw(GLError *error);

void gl_unpack_alignment(GLError *error);

void gl_blend_enable(GLError *error);
void gl_blend_disable(GLError *error);
void gl_blend_func(GLError *error, GLenum sfactor, GLenum dfactor);
void gl_depth_func(GLError *error, GLenum func);
