#pragma once
#include "str.h"

#define BNF_FAILURE 0
#define BNF_SUCCESS 1
#define BNF_END 2

typedef struct Rule Rule;
typedef struct Expression Expression;
typedef struct Term Term;

typedef struct Bnf {
	Str src;
	char* error;
	int line;
	Str rule;
	Rule* rules;
	int rules_count;
	Expression* expressions;
	int expressions_count;
	Term* terms;
	int terms_count;
} Bnf;

int bnf_parse(Bnf* bnf, const char* src);
void bnf_delete(Bnf* bnf);
Rule* bnf_get_rule(Bnf* bnf, Str* name);
void bnf_print(Bnf* bnf);