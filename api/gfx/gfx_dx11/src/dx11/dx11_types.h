#pragma once
#include "api/gfx/gfx_api_types.h"
#include "d3d11.h"

#include <dxgidebug.h>

typedef struct DX11Error {
	HMODULE library;
	IDXGIInfoQueue* info_queue;
	UINT64 begin;

	WStr caption;
	WStr text;
	WStr desc;
	Str info;

	LogCallbacks* log;
} DX11Error;