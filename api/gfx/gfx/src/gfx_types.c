#include "gfx_types.h"

#include "mem.h"

#include <memory.h>

ATypeType atype_type(AType type) {
	switch (type) {
	case VEC1B: case VEC2B: case VEC3B: case VEC4B: return A_B;
	case VEC1UB: case VEC2UB: case VEC3UB: case VEC4UB: return A_UB;
	case VEC1S: case VEC2S: case VEC3S: case VEC4S: return A_S;
	case VEC1US: case VEC2US: case VEC3US: case VEC4US: return A_US;
	case VEC1I: case VEC2I: case VEC3I: case VEC4I: return A_I;
	case VEC1UI: case VEC2UI: case VEC3UI: case VEC4UI: return A_UI;
	case VEC1F: case VEC2F: case VEC3F: case VEC4F: return A_F;
	case VEC1D: case VEC2D: case VEC3D: case VEC4D: return A_D;
	case MAT4F: return A_F;
	}
	return 0;
}

uint atype_components(AType type, bool mat_support) {
	switch (type) {
	case VEC1B: case VEC1UB: case VEC1S: case VEC1US: case VEC1I: case VEC1UI: case VEC1F: case VEC1D: return 1;
	case VEC2B: case VEC2UB: case VEC2S: case VEC2US: case VEC2I: case VEC2UI: case VEC2F: case VEC2D: return 2;
	case VEC3B: case VEC3UB: case VEC3S: case VEC3US: case VEC3I: case VEC3UI: case VEC3F: case VEC3D: return 3;
	case VEC4B: case VEC4UB: case VEC4S: case VEC4US: case VEC4I: case VEC4UI: case VEC4F: case VEC4D: return 4;
	case MAT4F: return mat_support == 0 ? 4 : 16;
	}
	return 0;
}

uint atype_component_size(AType type) {
	switch (atype_type(type)) {
	case A_B: return sizeof(char);
	case A_UB: return sizeof(unsigned char);
	case A_S: return sizeof(short);
	case A_US: return sizeof(unsigned short);
	case A_I: return sizeof(int);
	case A_UI: return sizeof(unsigned int);
	case A_F: return sizeof(float);
	case A_D: return sizeof(double);
	}
	return 0;
}

uint atype_size(AType type) {
	return atype_components(type, 1) * atype_component_size(type);
}

uint atype_count(AType type, bool mat_support) {
	switch (type) {
	case VEC1B: case VEC2B: case VEC3B: case VEC4B: case VEC1UB: case VEC2UB: case VEC3UB: case VEC4UB:
	case VEC1S: case VEC2S: case VEC3S: case VEC4S: case VEC1US: case VEC2US: case VEC3US: case VEC4US:
	case VEC1I: case VEC2I: case VEC3I: case VEC4I: case VEC1UI: case VEC2UI: case VEC3UI: case VEC4UI:
	case VEC1F: case VEC2F: case VEC3F: case VEC4F:
	case VEC1D: case VEC2D: case VEC3D: case VEC4D:
		return 1;
	case MAT4F:
		return mat_support == 0 ? 4 : 1;
	}
	return 0;
}

void atype_convert(float* dst, const void* src, AType type) {
	for (uint i = 0; i < atype_components(type, 1); i++) {
		switch (atype_type(type)) {
		case A_B: dst[i] = (float)((char*)src)[i]; break;
		case A_UB: dst[i] = (float)((unsigned char*)src)[i]; break;
		case A_S: dst[i] = (float)((short*)src)[i]; break;
		case A_US: dst[i] = (float)((unsigned short*)src)[i]; break;
		case A_I: dst[i] = (float)((int*)src)[i]; break;
		case A_UI: dst[i] = (float)((unsigned int*)src)[i]; break;
		case A_F: dst[i] = (float)((float*)src)[i]; break;
		case A_D: dst[i] = (float)((double*)src)[i]; break;
		}
	}
}

uint abufferdesc_size(ABufferDesc* desc) {
	uint count = desc->values_size / sizeof(AValue);
	uint size = 0;
	for (uint i = 0; i < count; i++) {
		size += atype_size(desc->values[i].type);
	}
	return size;
}

uint abufferdesc_count(ABufferDesc* desc, bool mat_support) {
	uint count = desc->values_size / sizeof(AValue);
	uint size = 0;
	for (uint i = 0; i < count; i++) {
		size += atype_count(desc->values[i].type, mat_support);
	}
	return size;
}

void abufferdesc_copy(ABufferDesc* src, ABufferDesc* dst) {
	memcpy(dst, src, sizeof(ABufferDesc));
	dst->values = m_malloc(src->values_size);
	memcpy(dst->values, src->values, src->values_size);
}

void abufferdesc_delete(ABufferDesc* desc) {
	uint values_count = desc->values_size / sizeof(AValue);
	m_free(desc->values, desc->values_size);
}

void ashaderdesc_copy(AShaderDesc* src, AShaderDesc* dst) {
	memcpy(dst, src, sizeof(AShaderDesc));
	dst->buffers = m_malloc(src->buffers_size);
	uint buffers_count = src->buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		abufferdesc_copy(&src->buffers[i], &dst->buffers[i]);
	}
}

void ashaderdesc_delete(AShaderDesc* desc) {
	uint buffers_count = desc->buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		abufferdesc_delete(&desc->buffers[i]);
	}
	m_free(desc->buffers, desc->buffers_size);
}

ABufferDesc* ashaderdesc_get_bufferdesc(AShaderDesc desc, ABufferType type) {
	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		if (desc.buffers[i].type == type) {
			return &desc.buffers[i];
		}
	}
	return NULL;
}