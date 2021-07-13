#pragma once
#ifdef E_OPENGL
#include "gl.h"
#include "api/gfx/aenums.h"

GLenum gl_aprimitive(APrimitive primitive);
GLint gl_awrap(AWrap wrap);
GLint gl_afilter(AFilter filter);
GLenum gl_afactor(AFactor factor);
#endif