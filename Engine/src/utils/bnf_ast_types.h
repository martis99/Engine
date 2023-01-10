#pragma once
#include "str.h"

typedef enum TermType {
	TERM_TYPE_RULE,
	TERM_TYPE_LITERAL,
	TERM_TYPE_IDENTIFIER,
	TERM_TYPE_INT,
	TERM_TYPE_FLOAT,
	TERM_TYPE_TAB,
	TERM_TYPE_NEW_LINE,
} TermType;

struct Term {
	TermType type;
	byte list;
	byte optional;
	byte recursive;
	byte one;
	Str text;
	Rule *rule;
	Expression *expression;
	Term *next;
};

struct Expression {
	Term *term;
	Expression *next;
};

typedef enum RuleType {
	RULE_TYPE_DEFAULT,
	RULE_TYPE_VARIABLE_V,
	RULE_TYPE_VARIABLE_I,
} RuleType;

struct Rule {
	Str name;
	RuleType type;
	Expression *expression;
};
