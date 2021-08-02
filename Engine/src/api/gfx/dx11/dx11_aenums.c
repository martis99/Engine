#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_aenums.h"

D3D11_PRIMITIVE_TOPOLOGY dx11_aprimitive(APrimitive primitive) {
	switch (primitive) {
	case A_POINTS: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	case A_LINES: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case A_TRIANGLES: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
	return 0;
}

D3D11_TEXTURE_ADDRESS_MODE dx11_awrap(AWrap wrap) {
	switch (wrap) {
	case A_REPEAT: return D3D11_TEXTURE_ADDRESS_WRAP;
	case A_CLAMP_TO_EDGE: return D3D11_TEXTURE_ADDRESS_CLAMP;
	}
	return 0;
}

D3D11_FILTER dx11_afilter(AFilter filter) {
	switch (filter) {
	case A_NEAREST: return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case A_LINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	return 0;
}

#endif
