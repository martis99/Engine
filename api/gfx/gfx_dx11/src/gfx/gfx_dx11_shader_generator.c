#include "api/gfx/gfx_api_shader_generator.h"

#include "gfx_dx11_types.h"

#include "utils/str.h"

void ashadergenerator_generate(AShaderDesc desc, Str* vert, Str* frag) {
	str_add_char(vert, '\0');
	printf("----------------------Vertex----------------------------\n");
	str_print(vert);

	str_add_char(frag, '\0');
	printf("----------------------Fragment----------------------------\n");
	str_print(frag);
}

const char* ashadergenerator_get_bnf() {
	return
		"<program>                ::= { <vs_in_struct> | <vs_out_struct> | <fs_in_struct> | <fs_out_struct> | <buffer_struct> | <function_definition> | <function_prototype> | <textures_declaration> }1\n"
		"<textures_declaration>   ::= 'Texture2D<float4> ' <identifier> '[' <int> '];' <new_line> 'SamplerState Samplers[' <int> '];' <new_line>\n"
		"<vs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
		"<vs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
		"<fs_in_struct>           ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
		"<fs_out_struct>          ::= 'struct ' <identifier> ' {' <new_line> { <struct_mem_sem> }1 '};' <new_line>\n"
		"<buffer_struct>          ::= 'cbuffer ' <identifier> ' : register (b' <int> ') {' <new_line> { <tab> <type> ' ' <identifier> ';' <new_line> } '};' <new_line>\n"
		"<struct_mem_sem>         ::= <tab> <type> ' ' <identifier> ' : ' <semantic> ';' <new_line> | <tab> <type> ' ' <identifier> ';' <new_line>\n"
		"<semantic>               ::= <identifier>\n"
		"<type>$i                 ::= 'void' | 'bool' | 'bool'  | 'int'   | 'int'    | 'int' | 'uint' | 'float' | 'double' | 'int1'  | 'int2'  | 'int3'  | 'int4'  | 'int1'   | 'int2'   | 'int3'   | 'int4'   | 'int1'  | 'int2'  | 'int3'  | 'int4'  | 'int1'   | 'int2'   | 'int3'   | 'int4'   | 'int1'  | 'int2'  | 'int3'  | 'int4'  | 'uint1'  | 'uint2'  | 'uint3'  | 'uint4'  | 'float1' | 'float2' | 'float3' | 'float4' | 'double1' | 'double2' | 'double3' | 'double4' | 'matrix'\n"
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
		"<value>                  ::= <float> | <int> | <negative> | <lvalue> '++' | <lvalue> '--' | <function_call> '.' <value> | <array_access> '.' <value> | <identifier> '.' <value> | <function_call> | <array_access> | <identifier>\n"
		"<array_access>           ::= <identifier> '[' <value> ']'\n"
		"<negative>               ::= '-' <value>\n"
		"<expression_operator>$v  ::= '+' | '-' | '*' | '/' | '<' | '>' | '==' | '>=' | '<='\n"
		"<assignment_operator>$v  ::= '=' | '+=' | '-=' | '*=' | '/='\n"
		"<lvalue>                 ::= <function_call> '.' <lvalue> | <array_access> '.' <lvalue>  | <identifier> '.' <lvalue> | <array_access> | <identifier>\n"
		"<function_call>          ::= <mul_call> | <sample_call> | <discard_call> | <type> '(' [ <arguments> ] ')' | <identifier> '(' [ <arguments> ] ')'\n"
		"<mul_call>               ::= 'mul(' <lexpression> ', ' <rexpression> ')'\n"
		"<lexpression>            ::= <expression>\n"
		"<rexpression>            ::= <expression>\n"
		"<sample_call>            ::= <identifier> '[' <int> '].Sample(Samplers[' <int> '], ' <expression> ')'\n"
		"<discard_call>           ::= 'discard;' <new_line>\n"
		"<arguments>              ::= <expression> ', ' <arguments> | <expression>\n"
		"\0";
}