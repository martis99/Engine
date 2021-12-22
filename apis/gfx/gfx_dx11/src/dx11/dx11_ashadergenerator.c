#include "api/gfx/ashadergenerator.h"
#include "dx11_atypes.h"

const char* get_type_token(AType type) {
	switch (type) {
	case VEC1B:	return NULL;
	case VEC2B:	return NULL;
	case VEC3B:	return NULL;
	case VEC4B:	return NULL;
	case VEC1UB: return NULL;
	case VEC2UB: return NULL;
	case VEC3UB: return NULL;
	case VEC4UB: return NULL;
	case VEC1S:	return NULL;
	case VEC2S:	return NULL;
	case VEC3S:	return NULL;
	case VEC4S:	return NULL;
	case VEC1US: return NULL;
	case VEC2US: return NULL;
	case VEC3US: return NULL;
	case VEC4US: return NULL;
	case VEC1I:	return "int";
	case VEC2I:	return "int2";
	case VEC3I:	return "int3";
	case VEC4I:	return "int4";
	case VEC1UI: return "uint";
	case VEC2UI: return "uint2";
	case VEC3UI: return "uint3";
	case VEC4UI: return "uint4";
	case VEC1F:	return "float";
	case VEC2F:	return "float2";
	case VEC3F:	return "float3";
	case VEC4F: return "float4";
	case VEC1D:	return NULL;
	case VEC2D: return NULL;
	case VEC3D:	return NULL;
	case VEC4D:	return NULL;
	case MAT4F: return "row_major matrix";
	}
	return "";
}

static void add_cbuffer(char* src, int* n, ABufferDesc desc) {
	appends(src, n, 3, "cbuffer ", desc.name, " : register (b");
	append_i(src, n, desc.slot);
	append(src, n, ") \n");
	add_values_block(src, n, desc, 0);
}

static void add_texture_sampler_function(char* src, int* n, AType return_type, const char* textues_name, const char* samplers_name, uint textures_count) {
	char* id = "id";
	char* coords = "coords";

	AValue args[] = {
		{VEC1I, "id"},
		{VEC2F, "coords"}
	};

	add_function_header(src, n, return_type, "sample_tex", args, sizeof(args));
	append(src, n, " {\n	");
	add_switch(src, n, id);
	append(src, n, " {\n");
	for (uint i = 0; i < textures_count; i++) {
		append(src, n, "	");
		add_case(src, n, i);
		append(src, n, "return ");
		add_array(src, n, textues_name, i);
		append(src, n, ".Sample(");
		add_array(src, n, samplers_name, i);
		appends(src, n, 3, ", ", coords, ");\n");
	}
	appends(src, n, 3, "	}\n	return ", get_type_token(return_type), "(");
	uint count = atype_components(return_type, 1);
	for (uint i = 0; i < count; i++) {
		append_i(src, n, 0);
		if (i < count - 1) {
			append(src, n, ",");
		}
	}

	append(src, n, ");\n}\n");
}

static void add_texture_sampler(char* src, int* n, AType return_type, const char* textures, const char* samplers, uint count) {

	add_array_declaration(src, n, "Texture2D", textures, count, get_type_token(return_type));
	add_array_declaration(src, n, "SamplerState", samplers, count, NULL);

	add_texture_sampler_function(src, n, return_type, textures, samplers, count);
}

static void generate_vertex_shader(AShaderDesc desc, char* src, const char* vert) {
	int n = 0;

	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		switch (desc.buffers[i].type) {
		case A_BFR_GLOBAL: add_cbuffer(src, &n, desc.buffers[i]); break;
		case A_BFR_VS: add_cbuffer(src, &n, desc.buffers[i]); break;
		case A_BFR_VERTEX: add_struct(src, &n, desc.buffers[i]); break;
		case A_BFR_INSTANCE: add_struct(src, &n, desc.buffers[i]); break;
		}
	}

	append(src, &n, vert);
}

static void add_ps_out(char* src, int* n, ABufferDesc desc) {
	appends(src, n, 3, "struct ", desc.name, " ");
	append(src, n, "{\n");
	uint values_count = desc.values_size / sizeof(AValue);
	for (uint i = 0; i < values_count; i++) {
		append(src, n, "	");
		add_type(src, n, desc.values[i].type);
		appends(src, n, 3, " ", desc.values[i].name, " : SV_Target");
		append_i(src, n, i);
		append(src, n, ";\n");
	}
	append(src, n, "};\n");
}

static void generate_fragment_shader(AShaderDesc desc, char* src, const char* frag) {
	int n = 0;

	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		switch (desc.buffers[i].type) {
		case A_BFR_PS: add_cbuffer(src, &n, desc.buffers[i]); break;
		case A_BFR_PS_OUT: add_ps_out(src, &n, desc.buffers[i]); break;
		}
	}

	if (desc.textures_count > 0) {
		add_texture_sampler(src, &n, desc.texture_type, "Textures", "Samplers", desc.textures_count);
	}

	append(src, &n, frag);
}


void ashadergenerator_generate(AShaderDesc desc, char* src_vert, char* src_frag, const char* vert, const char* frag) {
	generate_vertex_shader(desc, src_vert, vert);
	generate_fragment_shader(desc, src_frag, frag);
}