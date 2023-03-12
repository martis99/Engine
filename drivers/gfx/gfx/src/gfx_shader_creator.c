#include "gfx_shader_creator.h"
#include "assets/gfx_shader.h"

#include "utils/ast.h"
#include "utils/bnf.h"
#include "utils/str.h"

static Bnf *create_bnf(Bnf *bnf, const char *bnf_cstr)
{
	int r = bnf_parse(bnf, bnf_cstr);

	if (r != BNF_SUCCESS) {
		printf("BNF %i: %s rule: %.*s, line: %i\n", r, bnf->error, bnf->rule.count, bnf->rule.data, bnf->line + 1);
		return NULL;
	}

	return bnf;
}

ShaderCreator *gfx_sc_create(ShaderCreator *shader_creator, Renderer *renderer)
{
	const char *from_bnf =
		"<program>                ::= { <vs_in_struct> | <vs_out_struct> | <fs_in_struct> | <fs_out_struct> | <buffer_struct> | <function_definition> | <function_prototype> | <textures_declaration> }1\n"
		"<textures_declaration>   ::= 'textures ' <identifier> '[' <int> '];' <new_line> \n"
		"<vs_in_struct>           ::= 'vs_in' <int> ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
		"<vs_out_struct>          ::= 'vs_out' ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
		"<fs_in_struct>           ::= 'fs_in' ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
		"<fs_out_struct>          ::= 'fs_out' ' ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '}' <new_line>\n"
		"<buffer_struct>          ::= 'buffer' <int> ' ' <identifier> ' {' <new_line> { <tab> <type> ' ' <identifier> ';' <new_line> } '}' <new_line>\n"
		"<struct_mem_sem>         ::= <tab> <type> ' ' <identifier> ' : ' <semantic> ';' <new_line> | <tab> <type> ' ' <identifier> ';' <new_line>\n"
		"<semantic>               ::= <identifier>\n"
		"<type>$i                 ::= 'void' | 'byte' | 'ubyte' | 'short' | 'ushort' | 'int' | 'uint' | 'float' | 'double' | 'vec1b' | 'vec2b' | 'vec3b' | 'vec4b' | 'vec1ub' | 'vec2ub' | 'vec3ub' | 'vec4ub' | 'vec1s' | 'vec2s' | 'vec3s' | 'vec4s' | 'vec1us' | 'vec2us' | 'vec3us' | 'vec4us' | 'vec1i' | 'vec2i' | 'vec3i' | 'vec4i' | 'vec1ui' | 'vec2ui' | 'vec3ui' | 'vec4ui' | 'vec1f' | 'vec2f' | 'vec3f' | 'vec4f' | 'vec1d' | 'vec2d' | 'vec3d' | 'vec4d' | 'mat4f'\n"
		"<type_name>              ::= <type> | <identifier>\n"
		"<function_prototype>     ::= <function_header> ';' <new_line>\n"
		"<function_definition>    ::= <function_header> ' {' <new_line> { <tab> <compound> } '}' <new_line>\n"
		"<function_header>        ::= <type_name> ' ' <identifier> '(' [ <parameters> ] ')'\n"
		"<parameters>             ::= <parameter> ', ' <parameters> | <parameter>\n"
		"<parameter>              ::= <type_name> ' ' <identifier>\n"
		"<compound>               ::= <return_statement> | <if_statement> | <switch_statement> | <declaration_expression> | <assignment_expression> | <block> | <function_call>\n"
		"<return_statement>       ::= 'return ' <expression> ';' <new_line>\n"
		"<if_statement>           ::= 'if (' <expression> ') ' <block> ' else ' <if_statement> | 'if (' <expression> ') ' <block> ' else ' <compound> <new_line> | 'if (' <expression> ') ' <block> <new_line>\n"
		"<block>                  ::= '{' <new_line> { <tab> <tab> <compound> } <tab> '}'\n"
		"<switch_statement>       ::= 'switch (' <value> ') {' <new_line> { <tab> 'case ' <int> ': ' <compound> } <tab> '}' <new_line>\n"
		"<declaration_expression> ::= <type_name> ' ' <identifier> [ <initialization> ] ';' <new_line>\n"
		"<initialization>         ::= ' = ' <expression>\n"
		"<assignment_expression>  ::= <lvalue> ' ' <assignment_operator> ' ' <expression> ';' <new_line>\n"
		"<expression>             ::= <value> ' ' <expression_operator> ' ' <expression> | <paran_expression> ' ' <expression_operator> ' ' <expression> | <paran_expression> | <value>\n"
		"<paran_expression>       ::= '(' <expression> ')'\n"
		"<value>                  ::= <float> 'f' | <int> | <negative> | <lvalue> '++' | <lvalue> '--' | <function_call> '.' <value> | <array_access> '.' <value> | <identifier> '.' <value> | <function_call> | <array_access> | <identifier>\n"
		"<array_access>           ::= <identifier> '[' <value> ']'\n"
		"<negative>               ::= '-' <value>\n"
		"<expression_operator>$v  ::= '+' | '-' | '*' | '/' | '<' | '>' | '==' | '>=' | '<='\n"
		"<assignment_operator>$v  ::= '=' | '+=' | '-=' | '*=' | '/='\n"
		"<lvalue>                 ::= <function_call> '.' <lvalue> | <array_access> '.' <lvalue>  | <identifier> '.' <lvalue> | <array_access> | <identifier>\n"
		"<function_call>          ::= <mul_call> | <sample_call> | <discard_call> | <type> '(' [ <arguments> ] ')' | <identifier> '(' [ <arguments> ] ')'\n"
		"<mul_call>               ::= 'mul(' <lexpression> ', ' <rexpression> ')'\n"
		"<lexpression>            ::= <expression>\n"
		"<rexpression>            ::= <expression>\n"
		"<sample_call>            ::= 'sample(' <identifier> ', ' <int> ', ' <expression> ')'\n"
		"<discard_call>           ::= 'discard();' <new_line>\n"
		"<arguments>              ::= <expression> ', ' <arguments> | <expression>\n"
		"\0";

	str_create(&shader_creator->vert, 1600);
	str_create(&shader_creator->frag, 1600);
	str_create(&shader_creator->str_vert, 1600);
	str_create(&shader_creator->str_frag, 1600);

	if (create_bnf(&shader_creator->from_bnf, from_bnf) == NULL) {
		return NULL;
	}

	if (create_bnf(&shader_creator->to_bnf, renderer->driver->sg_get_bnf()) == NULL) {
		return NULL;
	}

	return shader_creator;
}

void gfx_sc_delete(ShaderCreator *shader_creator)
{
	bnf_delete(&shader_creator->from_bnf);
	bnf_delete(&shader_creator->to_bnf);
	str_delete(&shader_creator->vert);
	str_delete(&shader_creator->frag);
	str_delete(&shader_creator->str_vert);
	str_delete(&shader_creator->str_frag);
}

static Str *parse(ShaderCreator *shader_creator, const char *src, Str *dst)
{
	str_clear(dst);
	Ast ast;
	int r = ast_parse(&ast, &shader_creator->from_bnf, src);
	if (r != AST_SUCCESS) {
		printf("AST %i: %s\n", r, ast.error);
		return NULL;
	}

	r = ast_gen(&ast, &shader_creator->to_bnf, dst);
	if (r != AST_SUCCESS) {
		printf("AST %i: %s\n", r, ast.error);
		return NULL;
	}

	ast_delete(&ast);

	return dst;
}

static const char *get_type_token(AType type)
{
	switch (type) {
	case VEC1B: return "vec1b";
	case VEC2B: return "vec2b";
	case VEC3B: return "vec3b";
	case VEC4B: return "vec4b";
	case VEC1UB: return "vec1ub";
	case VEC2UB: return "vec2ub";
	case VEC3UB: return "vec3ub";
	case VEC4UB: return "vec4ub";
	case VEC1S: return "vec1s";
	case VEC2S: return "vec2s";
	case VEC3S: return "vec3s";
	case VEC4S: return "vec4s";
	case VEC1US: return "vec1us";
	case VEC2US: return "vec2us";
	case VEC3US: return "vec3us";
	case VEC4US: return "vec4us";
	case VEC1I: return "vec1i";
	case VEC2I: return "vec2i";
	case VEC3I: return "vec3i";
	case VEC4I: return "vec4i";
	case VEC1UI: return "vec1ui";
	case VEC2UI: return "vec2ui";
	case VEC3UI: return "vec3ui";
	case VEC4UI: return "vec4ui";
	case VEC1F: return "vec1f";
	case VEC2F: return "vec2f";
	case VEC3F: return "vec3f";
	case VEC4F: return "vec4f";
	case VEC1D: return "vec1d";
	case VEC2D: return "vec2d";
	case VEC3D: return "vec3d";
	case VEC4D: return "vec4d";
	case MAT4F: return "mat4f";
	}
	return "";
}

void vs_add_inputs(Str *str, AShaderDesc sdesc)
{
	int n		   = 0;
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_IN0 || bdesc.type == A_BFR_VS_IN1) {
			str_add_cstrf(str, "vs_in%i %s {\n", n, bdesc.name);
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s %s : %s;\n", get_type_token(bdesc.values[i].type), bdesc.values[i].name, bdesc.values[i].name);
			}
			str_add_cstr(str, "}\n", 0);
			n++;
		}
	}
}

void vs_add_buffers(Str *str, AShaderDesc sdesc)
{
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_GLOBAL || bdesc.type == A_BFR_VS) {
			str_add_cstrf(str, "buffer%i %s {\n", bdesc.slot, bdesc.name);
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s %s;\n", get_type_token(bdesc.values[i].type), bdesc.values[i].name);
			}
			str_add_cstr(str, "}\n", 0);
		}
	}
}

void vs_add_output(Str *str, AShaderDesc sdesc)
{
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_OUT) {
			str_add_cstrf(str, "vs_out %s {\n", bdesc.name);
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s %s : %s;\n", get_type_token(bdesc.values[i].type), bdesc.values[i].name, bdesc.values[i].name);
			}
			str_add_cstr(str, "}\n", 0);
		}
	}
}

void fs_add_input(Str *str, AShaderDesc sdesc)
{
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_OUT) {
			str_add_cstrf(str, "fs_in %s {\n", bdesc.name);
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s %s : %s;\n", get_type_token(bdesc.values[i].type), bdesc.values[i].name, bdesc.values[i].name);
			}
			str_add_cstr(str, "}\n", 0);
		}
	}
}

void fs_add_buffers(Str *str, AShaderDesc sdesc)
{
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_PS) {
			str_add_cstrf(str, "buffer%i %s {\n", bdesc.slot, bdesc.name);
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s %s;\n", get_type_token(bdesc.values[i].type), bdesc.values[i].name);
			}
			str_add_cstr(str, "}\n", 0);
		}
	}
}

void fs_add_output(Str *str, AShaderDesc sdesc)
{
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_PS_OUT) {
			str_add_cstrf(str, "fs_out %s {\n", bdesc.name);
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s %s : SV_TARGET%i;\n", get_type_token(bdesc.values[i].type), bdesc.values[i].name, i);
			}
			str_add_cstr(str, "}\n", 0);
		}
	}
}

void fs_add_textures(Str *str, AShaderDesc sdesc)
{
	if (sdesc.textures_count == 0) {
		return;
	}

	str_add_cstrf(str, "textures Textures[%i];\n", sdesc.textures_count);
	str_add_cstr(str, "vec4f sample_tex(float id, vec2f coords) {\n", 0);
	for (unsigned int i = 0; i < sdesc.textures_count; i++) {
		str_add_cstrf(str, "	if (id == %i) {\n", i);
		str_add_cstrf(str, "		return sample(Textures, %i, coords);\n", i);
		str_add_cstrf(str, "	}\n");
	}
	str_add_cstr(str, "	return vec4f(0, 0, 0, 0);\n", 0);
	str_add_cstr(str, "}\n", 0);
}

void vs_generate(Str *str, const char *vert, AShaderDesc desc)
{
	str_clear(str);
	vs_add_inputs(str, desc);
	vs_add_buffers(str, desc);
	vs_add_output(str, desc);
	fs_add_textures(str, desc);
	str_add_cstr(str, vert, 0);
}

void fs_generate(Str *str, const char *frag, AShaderDesc desc)
{
	str_clear(str);
	fs_add_input(str, desc);
	fs_add_buffers(str, desc);
	fs_add_output(str, desc);
	fs_add_textures(str, desc);
	str_add_cstr(str, frag, 0);
}

Shader *gfx_sc_create_shader(ShaderCreator *shader_creator, Shader *shader, Renderer *renderer, const char *vert_cstr, const char *frag_cstr, AShaderDesc desc)
{
	vs_generate(&shader_creator->vert, vert_cstr, desc);
	fs_generate(&shader_creator->frag, frag_cstr, desc);

	parse(shader_creator, shader_creator->vert.data, &shader_creator->str_vert);
	parse(shader_creator, shader_creator->frag.data, &shader_creator->str_frag);

	renderer->driver->sg_generate(desc, &shader_creator->str_vert, &shader_creator->str_frag);
	if (shader_create(shader, renderer, shader_creator->str_vert.data, shader_creator->str_frag.data, desc) == NULL) {
		return NULL;
	}

	return shader;
}
