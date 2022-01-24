#pragma once
#include "eng_common_types.h"

typedef struct Str {
	char* data;
	uint count;
	uint offset;
	byte view;
} Str;

typedef struct WStr {
	wchar* data;
	uint count;
} WStr;