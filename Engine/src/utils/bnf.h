#pragma once
#include "str_type.h"

#define BNF_FAILURE 0
#define BNF_SUCCESS 1
#define BNF_END 2

typedef struct Rule Rule;

typedef enum TermType {
	TERM_TYPE_RULE,
	TERM_TYPE_LITERAL,
	TERM_TYPE_IDENTIFIER,
	TERM_TYPE_INT,
	TERM_TYPE_FLOAT,
	TERM_TYPE_TAB,
	TERM_TYPE_NEW_LINE
} TermType;

typedef struct Expression Expression;

typedef struct Term Term;
struct Term {
	TermType type;
	byte list;
	byte optional;
	byte recursive;
	byte one;
	Str text;
	Rule* rule;
	Expression* expression;
	Term* next;
};

struct Expression {
	Term* term;
	Expression* next;
};

typedef enum RuleType {
	RULE_TYPE_DEFAULT,
	RULE_TYPE_VARIABLE_V,
	RULE_TYPE_VARIABLE_I
} RuleType;

struct Rule {
	Str name;
	RuleType type;
	Expression* expression;
};

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