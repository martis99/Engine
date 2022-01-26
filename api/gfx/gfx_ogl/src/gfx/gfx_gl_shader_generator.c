#include "api/gfx/gfx_api_shader_generator.h"

#include "gfx_gl_types.h"

#include "utils/str.h"

static const char* get_type_token(AType type) {
	switch (type) {
	case VEC1B:	return "bool";
	case VEC2B:	return "bvec2";
	case VEC3B:	return "bvec3";
	case VEC4B:	return "bvec4";
	case VEC1UB: return "bool";
	case VEC2UB: return "bvec2";
	case VEC3UB: return "bvec3";
	case VEC4UB: return "bvec4";
	case VEC1S:	return "int";
	case VEC2S:	return "ivec2";
	case VEC3S:	return "ivec3";
	case VEC4S:	return "ivec4";
	case VEC1US: return "int";
	case VEC2US: return "ivec2";
	case VEC3US: return "ivec3";
	case VEC4US: return "ivec4";
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
	case VEC1D:	return "double";
	case VEC2D: return "dvec2";
	case VEC3D:	return "dvec3";
	case VEC4D:	return "dvec4";
	case MAT4F: return "mat4";
	}
	return "";
}

static void vs_add_input(AShaderDesc desc, Str* str) {
	int location = 0;
	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		if (desc.buffers[i].type == A_BFR_VS_IN0 || desc.buffers[i].type == A_BFR_VS_IN1) {
			uint values_count = desc.buffers[i].values_size / sizeof(AValue);
			for (uint j = 0; j < values_count; j++) {
				str_add_cstrf(str, "layout (location = %i) in %s %s;\n", location++, get_type_token(desc.buffers[i].values[j].type), desc.buffers[i].values[j].name);
			}
		}
	}
}

static void vs_add_input_converters(AShaderDesc sdesc, Str* str) {
	int location = 0;
	int n = 0;
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_IN0 || bdesc.type == A_BFR_VS_IN1) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	g_in%i.%s = %s;\n", n, bdesc.values[i].name, bdesc.values[i].name);
			}
			n++;
		}
	}
}

static void vs_add_output_converters(AShaderDesc sdesc, Str* str) {
	int location = 0;
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_OUT) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	vs_out.%s = g_out.%s;\n", bdesc.values[i].name, bdesc.values[i].name);
			}
		}
	}
}

static void vs_add_parameters(AShaderDesc sdesc, Str* str) {
	int location = 0;
	int n = 0;
	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_IN0 || bdesc.type == A_BFR_VS_IN1) {
			if (n > 0) {
				str_add_cstr(str, ", ", 0);
			}
			str_add_cstrf(str, "g_in%i", n);
			n++;
		}
	}
}

static void fs_add_output(AShaderDesc desc, Str* str) {
	int location = 0;
	uint buffers_count = desc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		if (desc.buffers[i].type == A_BFR_PS_OUT) {
			uint values_count = desc.buffers[i].values_size / sizeof(AValue);
			for (uint j = 0; j < values_count; j++) {
				str_add_cstrf(str, "layout (location = %i) out %s %s;\n", location++, get_type_token(desc.buffers[i].values[j].type), desc.buffers[i].values[j].name);
			}
		}
	}
}

static void fs_add_input_converters(AShaderDesc sdesc, Str* str) {
	int location = 0;

	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_VS_OUT) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	g_in.%s = fs_in.%s;\n", bdesc.values[i].name, bdesc.values[i].name);
			}
		}
	}
}

static void fs_add_output_converters(AShaderDesc sdesc, Str* str) {
	int location = 0;

	uint buffers_count = sdesc.buffers_size / sizeof(ABufferDesc);
	for (uint i = 0; i < buffers_count; i++) {
		ABufferDesc bdesc = sdesc.buffers[i];
		if (bdesc.type == A_BFR_PS_OUT) {
			uint values_count = bdesc.values_size / sizeof(AValue);
			for (uint i = 0; i < values_count; i++) {
				str_add_cstrf(str, "	%s = g_out.%s;\n", bdesc.values[i].name, bdesc.values[i].name);
			}
		}
	}
}

void ashadergenerator_generate(AShaderDesc desc, Str* vert, Str* frag) {
	vs_add_input(desc, vert);
	str_add_cstr(vert, "void main() {\n", 0);
	vs_add_input_converters(desc, vert);
	str_add_cstr(vert, "	g_out = vs_main(", 0);
	vs_add_parameters(desc, vert);
	str_add_cstr(vert, ");\n", 0);
	vs_add_output_converters(desc, vert);
	str_add_cstr(vert,
		"	gl_Position = g_out.SV_Position;\n}\n", 0);
	str_add_char(vert, '\0');
	printf("----------------------Vertex----------------------------\n");
	str_print(vert);

	fs_add_output(desc, frag);
	str_add_cstr(frag, "void main() {\n", 0);
	fs_add_input_converters(desc, frag);
	str_add_cstr(frag, "	g_out = fs_main(g_in);\n", 0);
	fs_add_output_converters(desc, frag);
	str_add_cstr(frag, "}\n", 0);
	str_add_char(frag, '\0');

	printf("----------------------Fragment----------------------------\n");
	str_print(frag);
}

const char* ashadergenerator_get_bnf() {
	return
		"<program>                ::= '#version 440 core' <new_line> { <vs_in_struct> | <vs_out_struct> | <fs_in_struct> | <fs_out_struct> | <buffer_struct> | <function_definition> | <function_prototype> | <textures_declaration> }1\n"
		"<textures_declaration>   ::= 'uniform sampler2D ' <identifier> '[' <int> '];' <new_line>\n"
		"<vs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_in' <int> ';' <new_line>\n"
		"<vs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_out;' <new_line> 'layout (location = 0) out ' <identifier> ' vs_out;' <new_line>\n"
		"<fs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_in;' <new_line> 'layout (location = 0) in ' <identifier> ' fs_in;' <new_line>\n"
		"<fs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line> <identifier> ' g_out;' <new_line>\n"
		"<buffer_struct>          ::= 'layout (std140, binding = ' <int> ') uniform ' <identifier> ' {' <new_line> { <tab> <type> ' ' <identifier> ';' <new_line> } '};' <new_line>\n"
		"<struct_mem_sem>         ::= <tab> <type> ' ' <identifier> ';' <new_line>\n"
		"<semantic>               ::= <identifier>\n"
		"<type>$i                 ::= 'void' | 'bool' | 'bool'  | 'flat int' | 'flat int' | 'flat int' | 'flat uint' | 'float' | 'double' | 'bool'  | 'bvec2' | 'bvec3' | 'bvec4' | 'bool'   | 'bvec2'  | 'bvec3'  | 'bvec2'  | 'flat int' | 'flat ivec2' | 'flat ivec3' | 'flat ivec4' | 'flat int' | 'flat ivec2' | 'flat ivec3' | 'flat ivec4' | 'flat int' | 'flat ivec2' | 'flat ivec3' | 'flat ivec4' | 'flat uint' | 'flat uvec2' | 'flat uvec3' | 'flat uvec4' | 'float' | 'vec2'  | 'vec3 ' | 'vec4 ' | 'double' | 'dvec2' | 'dvec3' | 'dvec4' | 'mat4'\n"
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
		"<mul_call>               ::= <rexpression> ' * ' <lexpression> \n"
		"<lexpression>            ::= <expression>\n"
		"<rexpression>            ::= <expression>\n"
		"<sample_call>            ::= 'texture(' <identifier> '[' <int> '], ' <expression> ')'\n"
		"<discard_call>           ::= 'discard;' <new_line>\n"
		"<arguments>              ::= <expression> ', ' <arguments> | <expression>\n"
		"\0";
}