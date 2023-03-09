#pragma once
#include "api/gfx_api_types.h"

#include "dx11_ctx/dx11_ctx_types.h"

D3D11_FILTER dx11_afilter(AFilter filter);
bool dx11_mipmap_afilter(AFilter filter);
bool dx11_anisotropic_afilter(AFilter filter);
