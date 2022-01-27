#include "gl_types.h"
#include "gl_defines.h"

GLint gl_min_afilter(AFilter filter) {
	switch (filter) {
	case A_NEAREST: return GL_NEAREST;
	case A_LINEAR: return GL_LINEAR;
	case A_MIPMAP_NEAREST: case A_ANISOTROPIC_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
	case A_MIPMAP_LINEAR: case A_ANISOTROPIC_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
	}
	return 0;
}

GLint gl_mag_afilter(AFilter filter) {
	switch (filter) {
	case A_NEAREST: case A_MIPMAP_NEAREST: case A_ANISOTROPIC_NEAREST: return GL_NEAREST;
	case A_LINEAR: case A_MIPMAP_LINEAR: case A_ANISOTROPIC_LINEAR: return GL_LINEAR;
	}
	return 0;
}

bool gl_mipmap_afilter(AFilter filter) {
	switch (filter)
	{
	case A_MIPMAP_NEAREST: case A_MIPMAP_LINEAR: case A_ANISOTROPIC_NEAREST: case A_ANISOTROPIC_LINEAR: return 1;
	}
	return 0;
}

bool gl_anisotropic_afilter(AFilter filter) {
	switch (filter)
	{
	case A_ANISOTROPIC_NEAREST: case A_ANISOTROPIC_LINEAR: return 1;
	}
	return 0;
}