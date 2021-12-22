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
	case VEC1B: return DXGI_FORMAT_R8_SINT;
	case VEC2B: return DXGI_FORMAT_R8G8_SINT;
	case VEC3B: return 0;
	case VEC4B: return DXGI_FORMAT_R8G8B8A8_SINT;
	case VEC1UB: return DXGI_FORMAT_R8_UINT;
	case VEC2UB: return DXGI_FORMAT_R8G8_UINT;
	case VEC3UB: return 0;
	case VEC4UB: return DXGI_FORMAT_R8G8B8A8_UINT;
	case VEC1S: return DXGI_FORMAT_R16_SINT;
	case VEC2S: return DXGI_FORMAT_R16G16_SINT;
	case VEC3S: return 0;
	case VEC4S: return DXGI_FORMAT_R16G16B16A16_SINT;
	case VEC1US: return DXGI_FORMAT_R16_UINT;
	case VEC2US: return DXGI_FORMAT_R16G16_UINT;
	case VEC3US: return 0;
	case VEC4US: return DXGI_FORMAT_R16G16B16A16_UINT;
	case VEC1I: return DXGI_FORMAT_R32_SINT;
	case VEC2I: return DXGI_FORMAT_R32G32_SINT;
	case VEC3I: return DXGI_FORMAT_R32G32B32_SINT;
	case VEC4I: return DXGI_FORMAT_R32G32B32A32_SINT;
	case VEC1UI: return DXGI_FORMAT_R32_UINT;
	case VEC2UI: return DXGI_FORMAT_R32G32_UINT;
	case VEC3UI: return DXGI_FORMAT_R32G32B32_UINT;
	case VEC4UI: return DXGI_FORMAT_R32G32B32A32_UINT;
	case VEC1F: return DXGI_FORMAT_R32_FLOAT;
	case VEC2F: return DXGI_FORMAT_R32G32_FLOAT;
	case VEC3F: return DXGI_FORMAT_R32G32B32_FLOAT;
	case VEC4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case VEC1D: return 0;
	case VEC2D: return 0;
	case VEC3D: return 0;
	case VEC4D: return 0;
	case MAT4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	return 0;
}