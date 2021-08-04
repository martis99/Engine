#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_atypes.h"

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

DXGI_FORMAT dx11_atype_format(AType type) {
	switch (type) {
	case VEC1U: return DXGI_FORMAT_R32_UINT;
	case VEC2U: return DXGI_FORMAT_R32G32_UINT;
	case VEC3U: return DXGI_FORMAT_R32G32B32_UINT;
	case VEC4U: return DXGI_FORMAT_R32G32B32A32_UINT;
	case VEC1I: return DXGI_FORMAT_R32_SINT;
	case VEC2I: return DXGI_FORMAT_R32G32_SINT;
	case VEC3I: return DXGI_FORMAT_R32G32B32_SINT;
	case VEC4I: return DXGI_FORMAT_R32G32B32A32_SINT;
	case VEC1F: return DXGI_FORMAT_R32_FLOAT;
	case VEC2F: return DXGI_FORMAT_R32G32_FLOAT;
	case VEC3F: return DXGI_FORMAT_R32G32B32_FLOAT;
	case VEC4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case MAT4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	return 0;
}

#endif
