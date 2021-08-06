#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_atypes.h"

GLenum gl_aprimitive(APrimitive primitive) {
	switch (primitive) {
	case A_POINTS: return GL_POINTS;
	case A_LINES: return GL_LINES;
	case A_TRIANGLES: return GL_TRIANGLES;
	case A_QUADS: return GL_QUADS;
	}
	return 0;
}

GLenum gl_ashadertype(AShaderType type) {
	switch (type) {
	case A_FRAGMENT: return GL_FRAGMENT_SHADER;
	case A_VERTEX: return GL_VERTEX_SHADER;
	case A_GEOMETRY: return GL_GEOMETRY_SHADER;
	case A_COMPUTE: return GL_COMPUTE_SHADER;
	}
	return 0;
}

GLint gl_awrap(AWrap wrap) {
	switch (wrap) {
	case A_REPEAT: return GL_REPEAT;
	case A_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
	}
	return 0;
}

GLint gl_afilter(AFilter filter) {
	switch (filter) {
	case A_NEAREST: return GL_NEAREST;
	case A_LINEAR: return GL_LINEAR;
	}
	return 0;
}

GLenum gl_afactor(AFactor factor) {
	switch (factor) {
	case A_ZERO: return GL_ZERO;
	case A_ONE: return GL_ONE;
	case A_SRC_COLOR: return GL_SRC_COLOR;
	case A_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
	case A_SRC_ALPHA: return GL_SRC_ALPHA;
	case A_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	case A_DST_ALPHA: return  GL_DST_ALPHA;
	case A_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
	case A_DST_COLOR: return GL_DST_COLOR;
	case A_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
	}
	return 0;
}

GLenum gl_atype_type(AType type) {
	switch (atype_type(type)) {
	case A_B: return GL_BYTE;
	case A_UB: return GL_UNSIGNED_BYTE;
	case A_S: return GL_SHORT;
	case A_US: return GL_UNSIGNED_SHORT;
	case A_I: return GL_INT;
	case A_UI: return GL_UNSIGNED_INT;
	case A_F: return GL_FLOAT;
	case A_D: return GL_DOUBLE;
	}
	return 0;
}

GLint gl_atype_internal_format(AType type) {
	switch (type) {
	case VEC1B: return GL_R8I;
	case VEC2B: return GL_RG8I;
	case VEC3B: return GL_RGB8I;
	case VEC4B: return GL_RGBA8I;
	case VEC1UB: return GL_R8UI;
	case VEC2UB: return GL_RG8UI;
	case VEC3UB: return GL_RGB8UI;
	case VEC4UB: return GL_RGBA8UI;
	case VEC1S: return GL_R16I;
	case VEC2S: return GL_RG16I;
	case VEC3S: return GL_RGB16I;
	case VEC4S: return GL_RGBA16I;
	case VEC1US: return GL_R16UI;
	case VEC2US: return GL_RG16UI;
	case VEC3US: return GL_RGB16UI;
	case VEC4US: return GL_RGBA16UI;
	case VEC1I: return GL_R32I;
	case VEC2I: return GL_RG32I;
	case VEC3I: return GL_RGB32I;
	case VEC4I: return GL_RGBA32I;
	case VEC1UI: return GL_R32UI;
	case VEC2UI: return GL_RG32UI;
	case VEC3UI: return GL_RGB32UI;
	case VEC4UI: return GL_RGBA32UI;
	case VEC1F: return GL_R32F;
	case VEC2F: return GL_RG32F;
	case VEC3F: return GL_RGB32F;
	case VEC4F: return GL_RGBA32F;
	case VEC1D:
	case VEC2D:
	case VEC3D:
	case VEC4D:
	case MAT4F:
		return 0;
	}
	return 0;
}

GLenum gl_atype_format(AType type) {
	switch (type) {
	case VEC1B:
	case VEC1UB:
	case VEC1S:
	case VEC1US:
	case VEC1I:
		return GL_RED_INTEGER;
	case VEC2B:
	case VEC2UB:
	case VEC2S:
	case VEC2US:
	case VEC2I:
		return GL_RG_INTEGER;
	case VEC3B:
	case VEC3UB:
	case VEC3S:
	case VEC3US:
	case VEC3I:
		return GL_RGB_INTEGER;
	case VEC4B:
	case VEC4UB:
	case VEC4S:
	case VEC4US:
	case VEC4I:
		return GL_RGBA_INTEGER;
	case VEC1F:
	case VEC1D:
		return GL_RED;
	case VEC2F:
	case VEC2D:
		return GL_RG;
	case VEC3F:
	case VEC3D:
		return GL_RGB;
	case VEC4F:
	case VEC4D:
		return GL_RGBA;
	case MAT4F:
		return 0;
	}
	return 0;
}
#endif