#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/ashader.h"
#include "dx11_astructs.h"

#include <d3dcompiler.h>

#pragma comment (lib, "D3DCompiler.lib")

AShader* ashader_create(const char* src_vert, const char* src_frag, ARenderer* renderer) {
	AShader* shader = m_malloc(sizeof(AShader));

	const char* vert =
		"float4 main( float2 pos : POSITION ) : SV_POSITION {\n"
		"	return float4(pos.x, pos.y, 0.0f, 1.0f);\n"
		"}\0";

	const char* frag =
		"float4 main() : SV_TARGET {\n"
		"	return float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\0";

	HRESULT hr;
	hr = D3DCompile(vert, strlen(vert), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &shader->vs_blob, NULL);
	if (FAILED(hr)) {
		log_error("Failed to compile vertex shader");
		return NULL;
	}
	
	hr = renderer->device->lpVtbl->CreateVertexShader(renderer->device, shader->vs_blob->lpVtbl->GetBufferPointer(shader->vs_blob), shader->vs_blob->lpVtbl->GetBufferSize(shader->vs_blob), NULL, &shader->vs);
	if (FAILED(hr)) {
		log_error("Failed to create vertex shader");
		return NULL;
	}

	hr = D3DCompile(frag, strlen(frag), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &shader->ps_blob, NULL);
	if (FAILED(hr)) {
		log_error("Failed to compile pixel shader");
		return NULL;
	}
	
	hr = renderer->device->lpVtbl->CreatePixelShader(renderer->device, shader->ps_blob->lpVtbl->GetBufferPointer(shader->ps_blob), shader->ps_blob->lpVtbl->GetBufferSize(shader->ps_blob), NULL, &shader->ps);
	if (FAILED(hr)) {
		log_error("Failed to create pixel shader");
		return NULL;
	}
	return shader;
}

void ashader_delete(AShader* shader) {
	m_free(shader, sizeof(AShader));
}

void ashader_bind(AShader* shader, ARenderer* renderer) {
	renderer->context->lpVtbl->VSSetShader(renderer->context, shader->vs, NULL, 0);
	renderer->context->lpVtbl->PSSetShader(renderer->context, shader->ps, NULL, 0);
}

void ashader_bind_uniform_block(AShader* shader, const char* name, uint index) {

}

#endif