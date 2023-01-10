#include "bnf.h"

#include "bnf_ast_types.h"
#include "mem.h"

#define BNF_ERR_ALLOC		  300
#define BNF_ERR_TERM		  301
#define BNF_ERR_TERM_COUNT	  302
#define BNF_ERR_NAME_LENGTH	  303
#define BNF_ERR_RULE_NOT_FOUND	  304
#define BNF_ERR_EXPRESSIONS_COUNT 305

#define BNF_THROW(call)                     \
	do {                                \
		if (call == BNF_FAILURE) {  \
			return BNF_FAILURE; \
		}                           \
	} while (0)

#define BNF_THROW_IF(expression, message)     \
	do {                                  \
		if (expression) {             \
			bnf->error = message; \
			return BNF_FAILURE;   \
		}                             \
	} while (0)

#define BNF_MAX_RULES	    150
#define BNF_MAX_EXPRESSIONS 300
#define BNF_MAX_TERMS	    400

static int bnf_parse_rule_name(Bnf *bnf, Str *result, Str *src)
{
	Str name = str_read_alnum_(src);
	BNF_THROW_IF(name.count == 0, "Name length should be greater than 0");
	*result = name;
	return BNF_SUCCESS;
}

static int bnf_parse_text(Str *result, Str *src, char c)
{
	*result = str_read_until_char(src, c);
	return BNF_SUCCESS;
}

static int bnf_parse_literal(Bnf *bnf, Str *text, Str *src)
{
	if (str_get_char(src) == '"') {
		BNF_THROW_IF(str_read_char(src, '"') == 0, "Expected '\"'");
		BNF_THROW(bnf_parse_text(text, src, '"'));
		BNF_THROW_IF(str_read_char(src, '"') == 0, "Expected '\"'");
		return BNF_SUCCESS;
	}
	if (str_get_char(src) == '\'') {
		BNF_THROW_IF(str_read_char(src, '\'') == 0, "Expected '''");
		BNF_THROW(bnf_parse_text(text, src, '\''));
		BNF_THROW_IF(str_read_char(src, '\'') == 0, "Expected '''");
		return BNF_SUCCESS;
	}
	return BNF_END;
}

static int bnf_parse_opt_whitespace(Str *src)
{
	while (str_read_char(src, ' ') == ' ')
		;
	return BNF_SUCCESS;
}

static int bnf_parse_expressions(Bnf *bnf, Expression **expression, Str *src);

static int bnf_parse_term(Bnf *bnf, Term *term, Str *src)
{
	if (str_get_char(src) == '<') {
		BNF_THROW_IF(str_read_char(src, '<') == 0, "Expected '<'");
		BNF_THROW(bnf_parse_rule_name(bnf, &term->text, src));
		BNF_THROW_IF(str_read_char(src, '>') == 0, "Expected '>'");

		if (str_cmp_cstr(&term->text, "identifier", 0)) {
			term->type = TERM_TYPE_IDENTIFIER;
		} else if (str_cmp_cstr(&term->text, "int", 0)) {
			term->type = TERM_TYPE_INT;
		} else if (str_cmp_cstr(&term->text, "float", 0)) {
			term->type = TERM_TYPE_FLOAT;
		} else if (str_cmp_cstr(&term->text, "tab", 0)) {
			term->type = TERM_TYPE_TAB;
		} else if (str_cmp_cstr(&term->text, "new_line", 0)) {
			term->type = TERM_TYPE_NEW_LINE;
		} else {
			term->type = TERM_TYPE_RULE;
		}
		return BNF_SUCCESS;
	} else if (str_get_char(src) == '{') {
		BNF_THROW_IF(str_read_char(src, '{') == 0, "Expected '{'");
		BNF_THROW(bnf_parse_opt_whitespace(src));
		BNF_THROW(bnf_parse_expressions(bnf, &term->expression, src));
		BNF_THROW(bnf_parse_opt_whitespace(src));
		BNF_THROW_IF(str_read_char(src, '}') == 0, "Expected '}'");
		term->list = 1;
		term->one  = str_read_char(src, '1') == '1' ? 1 : 0;

		return BNF_SUCCESS;
	} else if (str_get_char(src) == '[') {
		BNF_THROW_IF(str_read_char(src, '[') == 0, "Expected '['");
		BNF_THROW(bnf_parse_opt_whitespace(src));
		BNF_THROW(bnf_parse_term(bnf, term, src));
		BNF_THROW(bnf_parse_opt_whitespace(src));
		BNF_THROW_IF(str_read_char(src, ']') == 0, "Expected ']'");
		term->optional = 1;
		return BNF_SUCCESS;
	} else if (bnf_parse_literal(bnf, &term->text, src) == BNF_SUCCESS) {
		term->type = TERM_TYPE_LITERAL;
		return BNF_SUCCESS;
	}
	return BNF_END;
}

static int bnf_parse_term_list(Bnf *bnf, Term **term, Str *src)
{
	int temp_count = bnf->terms_count;

	*term		= NULL;
	Term **parent	= term;
	Term *temp_term = &bnf->terms[bnf->terms_count++];

	while (1) {
		temp_term->list	    = 0;
		temp_term->optional = 0;
		int r		    = bnf_parse_term(bnf, temp_term, src);
		if (r != BNF_SUCCESS) {
			bnf->terms_count--;
			return r;
		}

		BNF_THROW(bnf_parse_opt_whitespace(src));
		*parent		= temp_term;
		temp_term->next = NULL;
		parent		= &temp_term->next;
		temp_term	= &bnf->terms[bnf->terms_count++];
	}
	return 3;
}

static int bnf_parse_expressions(Bnf *bnf, Expression **expression, Str *src)
{
	int temp_count		    = bnf->expressions_count;
	*expression		    = NULL;
	Expression **parent	    = expression;
	Expression *temp_expression = &bnf->expressions[bnf->expressions_count++];

	while (1) {
		int term_result = bnf_parse_term_list(bnf, &temp_expression->term, src);
		if (term_result != 2) {
			bnf->expressions_count = temp_count;
			return term_result;
		}
		*parent		      = temp_expression;
		temp_expression->next = NULL;
		parent		      = &temp_expression->next;
		temp_expression	      = &bnf->expressions[bnf->expressions_count++];
		BNF_THROW_IF(bnf->expressions_count > BNF_MAX_EXPRESSIONS, "Max expressions count reached");
		BNF_THROW(bnf_parse_opt_whitespace(src));
		if (str_read_char(src, '|') != '|') {
			bnf->expressions_count--;
			return BNF_END;
		}
		BNF_THROW(bnf_parse_opt_whitespace(src));
	}

	return 3;
}

static int bnf_parse_rule(Bnf *bnf, Rule *rule, Str *src)
{
	BNF_THROW(bnf_parse_opt_whitespace(src));
	BNF_THROW_IF(str_read_char(src, '<') == 0, "Expected '<'");
	BNF_THROW(bnf_parse_rule_name(bnf, &rule->name, src));
	BNF_THROW_IF(str_read_char(src, '>') == 0, "Expected '>'");

	if (str_read_char(src, '$') == '$') {
		if (str_read_char(src, 'v') == 'v') {
			rule->type = RULE_TYPE_VARIABLE_V;
		} else if (str_read_char(src, 'i') == 'i') {
			rule->type = RULE_TYPE_VARIABLE_I;
		} else {
			rule->type = RULE_TYPE_DEFAULT;
		}
	} else {
		rule->type = RULE_TYPE_DEFAULT;
	}

	BNF_THROW(bnf_parse_opt_whitespace(src));
	BNF_THROW_IF(str_read_char(src, ':') == 0, "Expected ':'");
	BNF_THROW_IF(str_read_char(src, ':') == 0, "Expected ':'");
	BNF_THROW_IF(str_read_char(src, '=') == 0, "Expected '='");

	BNF_THROW(bnf_parse_opt_whitespace(src));
	int r = bnf_parse_expressions(bnf, &rule->expression, src);
	if (r != BNF_END) {
		return r;
	}

	BNF_THROW_IF(rule->expression == NULL, "At least one expression is required");
	BNF_THROW_IF(str_read_char(src, '\n') == 0, "Expected line end");

	return BNF_SUCCESS;
}

static int bnf_parse_syntax(Bnf *bnf, Rule **rules, int *rules_count, Str *src)
{
	*rules_count = 0;

	int line = 0;
	while (!str_eos(src)) {
		Rule temp_rule = { 0 };

		int r = bnf_parse_rule(bnf, &temp_rule, src);
		if (r != BNF_SUCCESS) {
			bnf->line = line;
			bnf->rule = temp_rule.name;
			return r;
		}
		BNF_THROW_IF(*rules_count >= BNF_MAX_RULES, "Max rules count reached");
		(*rules)[(*rules_count)++] = temp_rule;
		line++;
	}
	return BNF_SUCCESS;
}

static int bnf_assign_rules_expression(Bnf *bnf, Rule *rule, int r, Expression *expression)
{
	Term *term = expression->term;
	while (term != NULL) {
		if (term->type == TERM_TYPE_RULE) {
			if (term->list == 1) {
				Expression *next = term->expression;
				while (next != NULL) {
					if (bnf_assign_rules_expression(bnf, rule, r, next) != BNF_SUCCESS) {
						return 0;
					}
					next = next->next;
				}
			} else {
				term->rule = bnf_get_rule(bnf, &term->text);
				if (term->rule == NULL) {
					bnf->error = "Rule not found";
					bnf->line  = r;
					bnf->rule  = rule->name;
					return 0;
				}
				term->recursive = term->rule == rule ? 1 : 0;
			}
		}
		term = term->next;
	}
	return BNF_SUCCESS;
}

static int bnf_assign_rules(Bnf *bnf)
{
	for (int r = 0; r < bnf->rules_count; r++) {
		Rule *rule	 = &bnf->rules[r];
		Expression *next = rule->expression;
		while (next != NULL) {
			if (bnf_assign_rules_expression(bnf, rule, r, next) != BNF_SUCCESS) {
				return 0;
			}
			next = next->next;
		}
	}
	return BNF_SUCCESS;
}

int bnf_parse(Bnf *bnf, const char *src)
{
	str_create_cstr(&bnf->src, src, 0);

	bnf->rules = m_calloc(BNF_MAX_RULES, sizeof(Rule));
	BNF_THROW_IF(bnf->rules == NULL, "Failed to allocate memory for rules");
	bnf->rules_count = 0;

	bnf->expressions = m_calloc(BNF_MAX_EXPRESSIONS, sizeof(Expression));
	BNF_THROW_IF(bnf->expressions == NULL, "Failed to allocate memory for expressions");
	bnf->expressions_count = 0;

	bnf->terms = m_calloc(BNF_MAX_TERMS, sizeof(Term));
	BNF_THROW_IF(bnf->terms == NULL, "Failed to allocate memory for terms");
	bnf->terms_count = 0;

	BNF_THROW(bnf_parse_syntax(bnf, &bnf->rules, &bnf->rules_count, &bnf->src));
	BNF_THROW(bnf_assign_rules(bnf));
	return BNF_SUCCESS;
}

void bnf_delete(Bnf *bnf)
{
	m_free(bnf->rules, BNF_MAX_RULES * sizeof(Rule));
	m_free(bnf->expressions, BNF_MAX_EXPRESSIONS * sizeof(Expression));
	m_free(bnf->terms, BNF_MAX_TERMS * sizeof(Term));
}

Rule *bnf_get_rule(Bnf *bnf, Str *name)
{
	for (int r = 0; r < bnf->rules_count; r++) {
		Rule *rule = &bnf->rules[r];
		if (str_cmp_str(&rule->name, name)) {
			return rule;
		}
	}

	return NULL;
}

static void bnf_print_expression(Expression *expression);

static void bnf_print_term(Term *term)
{
	if (term->optional == 1) {
		printf("[ ");
	}

	switch (term->type) {
	case TERM_TYPE_LITERAL:
		printf("'");
		str_print(&term->text);
		printf("'");
		break;
	default:
		printf("<");
		str_print(&term->text);
		printf(">");
		break;
	}

	if (term->optional == 1) {
		printf(" ]");
	}

	if (term->recursive == 1) {
		printf(" recursive");
	}

	if (term->list == 1) {
		printf("{ ");
		Expression *expression = term->expression;
		while (expression != NULL) {
			bnf_print_expression(expression);
			expression = expression->next;
			if (expression != NULL) {
				printf("| ");
			}
		}
		printf("}");
	}

	if (term->one == 1) {
		printf("1");
	}
}

static void bnf_print_expression(Expression *expression)
{
	Term *term = expression->term;
	while (term != NULL) {
		bnf_print_term(term);
		printf(" ");
		term = term->next;
	}
}

static void bnf_print_rule(Rule *rule)
{
	printf("<");
	str_print(&rule->name);
	printf(">");

	switch (rule->type) {
	case RULE_TYPE_VARIABLE_V: printf("$v"); break;
	case RULE_TYPE_VARIABLE_I: printf("$i"); break;
	}

	printf(" ::= ");

	Expression *next = rule->expression;
	while (next != NULL) {
		bnf_print_expression(next);
		next = next->next;
		if (next != NULL) {
			printf("| ");
		} else {
			printf("\n");
		}
	}
}

void bnf_print(Bnf *bnf)
{
	for (int i = 0; i < bnf->rules_count; i++) {
		bnf_print_rule(&bnf->rules[i]);
	}
}
