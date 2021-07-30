#include "pch.h"
#include "aenums.h"

uint type_size(AType type) {
	switch (type) {
	case EMPTY: return 0; break;
	case VEC1I: return 1 * sizeof(int);
	case VEC2I: return 2 * sizeof(int);
	case VEC3I: return 3 * sizeof(int);
	case VEC4I: return 4 * sizeof(int);
	case VEC1F: return 1 * sizeof(float);
	case VEC2F: return 2 * sizeof(float);
	case VEC3F: return 3 * sizeof(float);
	case VEC4F: return 4 * sizeof(float);
	case MAT4F: return 16 * sizeof(float);
	}
	return 0;
}

int type_count(AType type) {
	switch (type) {
	case VEC1I: return 1;
	case VEC2I: return 2;
	case VEC3I: return 3;
	case VEC4I: return 4;
	case VEC1F: return 1;
	case VEC2F: return 2;
	case VEC3F: return 3;
	case VEC4F: return 4;
	case MAT4F: return 16;
	default: return 0;
	}
	return 0;
}