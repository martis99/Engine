#pragma once
#include "api/gfx/gfx_api_types.h"
#include "d3d11.h"

#include "utils/str.h"

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

D3D11_FILTER dx11_afilter(AFilter filter);
bool dx11_mipmap_afilter(AFilter filter);
bool dx11_anisotropic_afilter(AFilter filter);