#pragma once
#ifdef GAPI_DX11
#include <d3d11.h>
#include "api/aenums.h"

D3D11_PRIMITIVE_TOPOLOGY dx11_aprimitive(APrimitive primitive);

D3D11_TEXTURE_ADDRESS_MODE dx11_awrap(AWrap wrap);
D3D11_FILTER dx11_afilter(AFilter filter);

#endif