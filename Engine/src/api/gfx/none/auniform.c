#include "pch.h"
#ifdef GAPI_NONE
#include "api/gfx/auniform.h"
#include "n_astructs.h"

AUniform* auniform_create(AShader* shader, const char* name, ADataType type, int count) {
	AUniform* uniform = m_malloc(sizeof(AUniform));
	return uniform;
}

void auniform_delete(AUniform* uniform) {
	m_free(uniform, sizeof(AUniform));
}

void auniform_set(AUniform* uniform, const void* data) {

}

void* auniform_get(AUniform* uniform) {
	return uniform->data;
}

size_t auniform_size(AUniform* uniform) {
	return 0;
}

void auniform_upload(AUniform* uniform) {
	switch (uniform->type) {
	case EMPTY: break;
	case VEC1I: break;
	case VEC2I: break;
	case VEC3I: break;
	case VEC4I: break;
	case VEC1F: break;
	case VEC2F: break;
	case VEC3F: break;
	case VEC4F: break;
	case MAT4F: break;
	default: break;
	}
}

void auniform_upload_vec1i(AUniform* uniform, int count, const void* data) {

}

void auniform_upload_mat4f(AUniform* uniform, int count, const void* data) {

}
#endif