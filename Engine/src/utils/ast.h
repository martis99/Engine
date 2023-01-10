#pragma once
#include "bnf.h"
#include "str.h"

#define AST_FAILURE   0
#define AST_SUCCESS   1
#define AST_END	      2
#define AST_RECURSION 3

typedef struct AstNode AstNode;

typedef struct Ast {
	Str src;
	char *error;
	AstNode *nodes;
	int nodes_count;
	AstNode *node;
} Ast;

AstNode *ast_get_node(AstNode *node, Str *name, int one);
int ast_parse(Ast *ast, Bnf *bnf, const char *src);
void ast_delete(Ast *ast);
int ast_gen(Ast *ast, Bnf *bnf, Str *dst);
void ast_print_node(AstNode *node, int depth);
void ast_print(Ast *ast);
