#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_shader.h"

#include <d3dcompiler.h>

#pragma comment (lib, "D3DCompiler.lib")

ID3D11VertexShader* dx11_vs_create(ID3D11Device* device, const char* src, ID3DBlob** blob) {
	ID3D11VertexShader* vs;

	HRESULT hr;
	hr = D3DCompile(src, strlen(src), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, blob, NULL);
	if (FAILED(hr)) {
		log_error("Failed to compile vertex shader");
		return NULL;
	}

	hr = device->lpVtbl->CreateVertexShader(device, (*blob)->lpVtbl->GetBufferPointer(*blob), (*blob)->lpVtbl->GetBufferSize(*blob), NULL, &vs);
	if (FAILED(hr)) {
		log_error("Failed to create vertex shader");
		return NULL;
	}

	return vs;
}

void dx11_vs_bind(ID3D11VertexShader* vs, ID3D11DeviceContext* context) {
	context->lpVtbl->VSSetShader(context, vs, NULL, 0);
}

void dx11_vs_delete(ID3D11VertexShader* vs, ID3DBlob* blob) {
	if (vs != NULL) {
		vs->lpVtbl->Release(vs);
	}
	if (blob != NULL) {
		blob->lpVtbl->Release(blob);
	}
}

ID3D11PixelShader* dx11_ps_create(ID3D11Device* device, const char* src, ID3DBlob** blob) {
	ID3D11PixelShader* ps;

	HRESULT hr;
	hr = D3DCompile(src, strlen(src), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, blob, NULL);
	if (FAILED(hr)) {
		log_error("Failed to compile vertex shader");
		return NULL;
	}

	hr = device->lpVtbl->CreatePixelShader(device, (*blob)->lpVtbl->GetBufferPointer(*blob), (*blob)->lpVtbl->GetBufferSize(*blob), NULL, &ps);
	if (FAILED(hr)) {
		log_error("Failed to create vertex shader");
		return NULL;
	}

	return ps;
}

void dx11_ps_bind(ID3D11PixelShader* ps, ID3D11DeviceContext* context) {
	context->lpVtbl->PSSetShader(context, ps, NULL, 0);
}

void dx11_ps_delete(ID3D11PixelShader* ps, ID3DBlob* blob) {
	if (ps != NULL) {
		ps->lpVtbl->Release(ps);
	}
	if (blob != NULL) {
		blob->lpVtbl->Release(blob);
	}
}
#endif