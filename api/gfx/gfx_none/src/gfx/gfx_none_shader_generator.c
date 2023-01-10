#include "api/gfx/gfx_api_shader_generator.h"

#include "gfx_none_types.h"

void ashadergenerator_generate(AShaderDesc desc, Str *vert, Str *frag)
{
}

const char *ashadergenerator_get_bnf()
{
	return "<program>                ::= { <vs_in_struct> | <vs_out_struct> | <fs_in_struct> | <fs_out_struct> | <buffer_struct> | <function_definition> | <function_prototype> | <textures_declaration> }1\n"
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
}
