#include "pch.h"
#include "atypes.h"

int atype_components(AType type, bool mat_support) {
	switch (type) {
	case VEC1U:
	case VEC1I:
	case VEC1F:
		return 1;
	case VEC2U:
	case VEC2I:
	case VEC2F:
		return 2;
	case VEC3U:
	case VEC3I:
	case VEC3F:
		return 3;
	case VEC4U:
	case VEC4I:
	case VEC4F:
		return 4;
	case MAT4F:
		return mat_support == 0 ? 4 : 16;
	}
	return 0;
}

uint atype_component_size(AType type) {
	switch (type) {
	case VEC1U:
	case VEC2U:
	case VEC3U:
	case VEC4U:
		return sizeof(unsigned int);
	case VEC1I:
	case VEC2I:
	case VEC3I:
	case VEC4I:
		return sizeof(int);
	case VEC1F:
	case VEC2F:
	case VEC3F:
	case VEC4F:
	case MAT4F:
		return sizeof(float);
	}
	return 0;
}

uint atype_size(AType type) {
	return atype_components(type, 1) * atype_component_size(type);
}

uint atype_count(AType type, bool mat_support) {
	switch (type) {
	case VEC1U: return 1;
	case VEC2U: return 1;
	case VEC3U: return 1;
	case VEC4U: return 1;
	case VEC1I: return 1;
	case VEC2I: return 1;
	case VEC3I: return 1;
	case VEC4I: return 1;
	case VEC1F: return 1;
	case VEC2F: return 1;
	case VEC3F: return 1;
	case VEC4F: return 1;
	case MAT4F: return mat_support == 0 ? 4 : 1;
	}
	return 0;
}

uint abufferdesc_size(ABufferDesc desc) {
	uint count = desc.layout_size / sizeof(AValue);
	uint size = 0;
	for (uint i = 0; i < count; i++) {
		size += atype_size(desc.layout[i].type);
	}
	return size;
}

uint abufferdesc_count(ABufferDesc desc, bool mat_support) {
	uint count = desc.layout_size / sizeof(AValue);
	uint size = 0;
	for (uint i = 0; i < count; i++) {
		size += atype_count(desc.layout[i].type, mat_support);
	}
	return size;
}

void abufferdesc_copy(ABufferDesc* dst, ABufferDesc* src) {
	if (src->enabled == 0) {
		return;
	}
	dst->layout = m_malloc(src->layout_size);
	memcpy(dst->layout, src->layout, src->layout_size);
}

void abufferdesc_delete(ABufferDesc* desc) {
	if (desc->enabled == 0) {
		return;
	}
	m_free(desc->layout, desc->layout_size);
}

void ameshdesc_copy(AMeshDesc* dst, AMeshDesc* src) {
	abufferdesc_copy(&dst->vertices, &src->vertices);
	abufferdesc_copy(&dst->instances, &src->instances);
	abufferdesc_copy(&dst->indices, &src->indices);
}

void ameshdesc_delete(AMeshDesc* desc) {
	abufferdesc_delete(&desc->vertices);
	abufferdesc_delete(&desc->instances);
	abufferdesc_delete(&desc->indices);
}