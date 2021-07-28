#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/ashader.h"
#include "dx11_astructs.h"

#include <d3dcompiler.h>

#pragma comment (lib, "D3DCompiler.lib")

AShader* ashader_create(const char* src_vert, const char* src_frag, ARenderer* renderer) {
	AShader* shader = m_malloc(sizeof(AShader));

	const char* vert =
		"cbuffer CBuf {\n"
		"	matrix transform;\n"
		"};\n"
		"struct VSOut {\n"
		"	float2 tex : TexCoord;\n"
		"	float4 pos : SV_Position;\n"
		"};\n"
		"VSOut main( float3 pos : Position, float2 tex : TexCoord ) {\n"
		"	VSOut vso;\n"
		"	vso.pos = mul(float4(pos, 1.0f), transform);\n"
		"	vso.tex = tex;\n"
		"	return vso;\n"
		"}\0";

	const char* frag =
		"Texture2D tex;\n"
		"SamplerState splr;\n"
		"float4 main(float2 tc : TexCoord) : SV_TARGET {\n"
		"	return tex.Sample(splr, tc);\n"
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
	renderer->context->lpVtbl->VSSetShader(renderer->context, shader->vs, NULL, 0);

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
	renderer->context->lpVtbl->PSSetShader(renderer->context, shader->ps, NULL, 0);

	typedef struct ConstantBuffer {
		mat4 transformation;
	} ConstantBuffer;

	ConstantBuffer cb;
	cb.transformation = mat4_invert(mat4_translation((vec3) { 0.5, 0, 0 }));

	ID3D11Buffer* constantBuffer;
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA csd = { 0 };
	csd.pSysMem = &cb;

	renderer->device->lpVtbl->CreateBuffer(renderer->device, &cbd, &csd, &constantBuffer);

	renderer->context->lpVtbl->VSSetConstantBuffers(renderer->context, 0, 1, &constantBuffer);

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