#pragma once
#include "api/gfx_api_types.h"

#include "gl_ctx/gl_ctx_types.h"

GLint gl_min_afilter(AFilter filter);
GLint gl_mag_afilter(AFilter filter);
bool gl_mipmap_afilter(AFilter filter);
bool gl_anisotropic_afilter(AFilter filter);
