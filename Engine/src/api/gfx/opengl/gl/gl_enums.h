#pragma once
#ifdef GAPI_OPENGL
#include "gl.h"
#include "api/aenums.h"

GLenum gl_aprimitive(APrimitive primitive);
GLenum gl_ashadertype(AShaderType type);
GLint gl_awrap(AWrap wrap);
GLint gl_afilter(AFilter filter);
GLenum gl_afactor(AFactor factor);
#endif