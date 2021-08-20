#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/ashadergenerator.h"
#include "gl_atypes.h"

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
	case VEC2I:	return "ivec2";
	case VEC3I:	return "ivec3";
	case VEC4I:	return "ivec4";
	case VEC1UI: return "uint";
	case VEC2UI: return "uvec2";
	case VEC3UI: return "uvec3";
	case VEC4UI: return "uvec4";
	case VEC1F:	return "float";
	case VEC2F:	return "vec2";
	case VEC3F:	return "vec3";
	case VEC4F: return "vec4";
	case VEC1D:	return NULL;
	case VEC2D: return NULL;
	case VEC3D:	return NULL;
	case VEC4D:	return NULL;
	case MAT4F: return "mat4";
	}
	return "";
}

static void add_cbuffer(char* src, int* n, ABufferDesc desc) {
	append(src, n, "layout (std140, binding = ");
	append_i(src, n, desc.slot);
	appends(src, n, 3, ") uniform ", desc.name, " ");
	add_values_block(src, n, desc, 0);
}

static void add_vs_input(char* src, int* n, ABufferDesc desc, int* location) {
	uint values_count = desc.values_size / sizeof(AValue);
	for (uint i = 0; i < values_count; i++) {
		append(src, n, "layout (location = ");
		append_i(src, n, (*location)++);
		append(src, n, ") in ");
		add_value(src, n, desc.values[i], 0);
		append(src, n, ";\n");
	}
}

static void add_ps_output(char* src, int* n, ABufferDesc desc) {
	uint values_count = desc.values_size / sizeof(AValue);
	for (uint i = 0; i < values_count; i++) {
		append(src, n, "layout (location = ");
		append_i(src, n, i);
		append(src, n, ") out ");
		add_value(src, n, desc.values[i], 0);
		append(src, n, ";\n");
	}
}

static void add_texture_sampler_function(char* src, int* n, AType return_type, const char* textues_name, uint textures_count) {
	char* id = "id";
	char* coords = "coords";

	AValue args[] = {
		{id, VEC1I},
		{coords, VEC2F}
	};

	add_function_header(src, n, return_type, "sample_tex", args, sizeof(args));
	append(src, n, " {\n	");
	add_switch(src, n, id);
	append(src, n, " {\n");
	for (uint i = 0; i < textures_count; i++) {
		append(src, n, "	");
		add_case(src, n, i);
		append(src, n, "return texture(");
		add_array(src, n, textues_name, i);
		appends(src, n, 3, ", ", coords, ");\n");
	}
	append(src, n, "	}\n}\n");
}

static void add_texture_sampler(char* src, int* n, AType return_type, const char* textures, uint count) {
	append(src, n, "uniform ");
	add_array_declaration(src, n, "sampler2D", textures, count, NULL);

	add_texture_sampler_function(src, n, return_type, textures, count);
}

static void generate_vertex_shader(AShaderDesc desc, char* src, const char* vert) {
	int n = 0;
	int location = 0;

	append(src, &n, "#version 440 core\n");
	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		switch (desc.buffers[i].type) {
		case A_BFR_GLOBAL: add_cbuffer(src, &n, desc.buffers[i]); break;
		case A_BFR_VS: add_cbuffer(src, &n, desc.buffers[i]); break;
		case A_BFR_VERTEX: add_vs_input(src, &n, desc.buffers[i], &location); break;
		case A_BFR_INSTANCE: add_vs_input(src, &n, desc.buffers[i], &location); break;
		}
	}

	append(src, &n, vert);
}

static void generate_fragment_shader(AShaderDesc desc, char* src, const char* frag) {
	int n = 0;

	append(src, &n, "#version 440 core\n");
	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		switch (desc.buffers[i].type) {
		case A_BFR_PS: add_cbuffer(src, &n, desc.buffers[i]); break;
		case A_BFR_PS_OUT: add_ps_output(src, &n, desc.buffers[i]); break;
		}
	}

	if (desc.textures_count > 0) {
		add_texture_sampler(src, &n, desc.texture_type, "Textures", desc.textures_count);
	}

	append(src, &n, frag);
}


void ashadergenerator_generate(AShaderDesc desc, char* src_vert, char* src_frag, const char* vert, const char* frag) {
	generate_vertex_shader(desc, src_vert, vert);
	generate_fragment_shader(desc, src_frag, frag);
}

#endif