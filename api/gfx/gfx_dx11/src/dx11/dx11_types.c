#include "dx11_types.h"

D3D11_FILTER dx11_afilter(AFilter filter) {
	switch (filter) {
	case A_NEAREST: case A_MIPMAP_NEAREST: return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case A_LINEAR: case A_MIPMAP_LINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	case A_ANISOTROPIC_NEAREST: case A_ANISOTROPIC_LINEAR: return D3D11_FILTER_ANISOTROPIC;
	}
	return 0;
}

bool dx11_mipmap_afilter(AFilter filter) {
	switch (filter)
	{
	case A_MIPMAP_NEAREST: case A_MIPMAP_LINEAR: case A_ANISOTROPIC_NEAREST: case A_ANISOTROPIC_LINEAR: return 1;
	}
	return 0;
}

bool dx11_anisotropic_afilter(AFilter filter) {
	switch (filter)
	{
	case A_ANISOTROPIC_NEAREST: case A_ANISOTROPIC_LINEAR: return 1;
	}
	return 0;
}