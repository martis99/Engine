#include "dx11_shader.h"
#include "dx11_error.h"

#include <d3dcompiler.h>

#pragma comment (lib, "D3DCompiler.lib")

ID3D11VertexShader* dx11_vs_create(DX11Error* error, ID3D11Device* device, const char* src, ID3DBlob** blob) {
	ID3D11VertexShader* vs = NULL;
	ID3DBlob* error_blob = NULL;

	if (DX11_FAILED(error, "Failed to compile vertex shader", D3DCompile(src, strlen(src), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, blob, &error_blob))) {
		if (error_blob) {
			printf("%s\n", (char*)error_blob->lpVtbl->GetBufferPointer(error_blob));
		}
		return NULL;
	}

	if (DX11_FAILED(error, "Failed to create vertex shader", device->lpVtbl->CreateVertexShader(device, (*blob)->lpVtbl->GetBufferPointer(*blob), (*blob)->lpVtbl->GetBufferSize(*blob), NULL, &vs))) {
		return NULL;
	}

	return vs;
}

void dx11_vs_bind(ID3D11VertexShader* vs, ID3D11DeviceContext* context) {
	context->lpVtbl->VSSetShader(context, vs, NULL, 0);
}

void dx11_vs_delete(ID3D11VertexShader* vs, ID3DBlob* blob) {
	vs->lpVtbl->Release(vs);
	blob->lpVtbl->Release(blob);
}

ID3D11PixelShader* dx11_ps_create(DX11Error* error, ID3D11Device* device, const char* src, ID3DBlob** blob) {
	ID3D11PixelShader* ps = NULL;
	ID3DBlob* error_blob = NULL;

	if (DX11_FAILED(error, "Failed to compile pixel shader", D3DCompile(src, strlen(src), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, blob, &error_blob))) {
		if (error_blob) {
			printf("%s\n", (char*)error_blob->lpVtbl->GetBufferPointer(error_blob));
		}
		return NULL;
	}

	if (DX11_FAILED(error, "Failed to create pixel shader", device->lpVtbl->CreatePixelShader(device, (*blob)->lpVtbl->GetBufferPointer(*blob), (*blob)->lpVtbl->GetBufferSize(*blob), NULL, &ps))) {
		return NULL;
	}

	return ps;
}

void dx11_ps_bind(ID3D11PixelShader* ps, ID3D11DeviceContext* context) {
	context->lpVtbl->PSSetShader(context, ps, NULL, 0);
}

void dx11_ps_delete(ID3D11PixelShader* ps, ID3DBlob* blob) {
	ps->lpVtbl->Release(ps);
	blob->lpVtbl->Release(blob);
}