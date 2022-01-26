#include "ast.h"

#include "mem.h"

#include "bnf_ast_types.h"
#include "bnf.h"

typedef enum AstNodeType {
	ASTNODE_TYPE_PARENT,
	ASTNODE_TYPE_LIST,
	ASTNODE_TYPE_VARIABLE_V,
	ASTNODE_TYPE_VARIABLE_I,
	ASTNODE_TYPE_IDENTIFIER,
	ASTNODE_TYPE_INT,
	ASTNODE_TYPE_FLOAT
} AstNodeType;

struct AstNode {
	AstNodeType type;
	Str name;
	Str text;
	int i;
	AstNode* next;
	AstNode* child;
};

#define AST_ERROR(message) ast->error = message; return AST_FAILURE;
#define AST_THROW(call) if (call == AST_FAILURE) { return AST_FAILURE; }
#define AST_THROW_IF(expression, message) if (expression) { ast->error = message; return AST_FAILURE; }

#define AST_MAX_NODES 1500

static int ast_parse_rule(Ast* ast, AstNode* node, Rule* rule, Str* src);

static int ast_parse_term(Ast* ast, AstNode* node, Term* term, Str* rule_name, int* add, Str* src) {
	switch (term->type) {
	case TERM_TYPE_RULE:
		*add = 1;
		node->type = ASTNODE_TYPE_PARENT;
		node->name = term->text;
		return ast_parse_rule(ast, node, term->rule, src);
	case TERM_TYPE_LITERAL:
		return str_read_str(src, &term->text).count > 0 ? AST_SUCCESS : AST_END;
	case TERM_TYPE_IDENTIFIER:
	{
		Str name = str_read_alnum_(src);
		if (name.count == 0) {
			return AST_END;
		}
		*add = 1;
		node->name = term->text;
		node->type = ASTNODE_TYPE_IDENTIFIER;
		node->text = name;
		node->child = NULL;
		return AST_SUCCESS;
	}
	case TERM_TYPE_INT:
	{
		Str number = str_read_num(src);
		if (number.count == 0) {
			return AST_END;
		}
		*add = 1;
		node->name = term->text;
		node->type = ASTNODE_TYPE_INT;
		node->text = number;
		node->child = NULL;
		return AST_SUCCESS;
	}
	case TERM_TYPE_FLOAT:
	{
		Str number = str_read_float(src);
		if (number.count == 0) {
			return AST_END;
		}
		*add = 1;
		node->name = term->text;
		node->type = ASTNODE_TYPE_FLOAT;
		node->text = number;
		node->child = NULL;
		return AST_SUCCESS;
	}
	case TERM_TYPE_TAB:
		return str_read_char(src, '\t') != 0 ? AST_SUCCESS : AST_END;
	case TERM_TYPE_NEW_LINE:
		return str_read_char(src, '\n') != 0 ? AST_SUCCESS : AST_END;
	}

	AST_ERROR("Unkown term type");
}

static int ast_parse_expression(Ast* ast, AstNode* node, Expression* expression, Str* rule_name, int add, Str* src, int one);

static int ast_parse_term_mod(Ast* ast, AstNode* node, Term* term, Str* rule_name, int* add, Str* src) {
	if (term->list == 1) {
		node->type = ASTNODE_TYPE_LIST;
		node->name = term->text;
		node->child = NULL;
		*add = 1;
		node->child = NULL;
		AstNode** parent = &node->child;
		int temp_count = ast->nodes_count;
		AstNode* temp_node = &ast->nodes[ast->nodes_count++];
		AST_THROW_IF(ast->nodes_count > AST_MAX_NODES, "Max number of nodes reached");

		while (1) {
			Expression* next = term->expression;
			int found = 0;
			while (next != NULL) {
				Str temp_src = *src;
				int r = ast_parse_expression(ast, temp_node, next, rule_name, 0, &temp_src, term->one);
				if (r == AST_SUCCESS) {
					*parent = temp_node;
					temp_node->next = NULL;
					parent = &temp_node->next;
					temp_node = &ast->nodes[ast->nodes_count++];
					*temp_node = (AstNode){ 0 };
					AST_THROW_IF(ast->nodes_count > AST_MAX_NODES, "Max number of nodes reached");
					*src = temp_src;
					found = 1;
					break;
				} else if (r == AST_FAILURE) {
					return r;
				}
				next = next->next;
			}
			if (found == 0) {
				break;
			}
		}

		if (node->child == NULL) {
			ast->nodes_count = temp_count;
			return AST_END;
		}

		ast->nodes_count--;
		return AST_SUCCESS;
	} else {
		int r = ast_parse_term(ast, node, term, rule_name, add, src);
		if (term->optional == 1) {
			return AST_SUCCESS;
		} else {
			return r;
		}
	}
}

static int ast_parse_term_list(Ast* ast, AstNode* node, Term* term, Str* rule_name, int add, Str* src, int one) {
	Str temp_src = *src;

	if (one == 1) {
		int temp_add = add;
		int r = ast_parse_term_mod(ast, node, term, rule_name, &temp_add, &temp_src);
		if (r == AST_SUCCESS) {
			*src = temp_src;
		} else {
			return r;
		}
	} else {
		node->child = NULL;
		AstNode** parent = &node->child;
		int temp_count = ast->nodes_count;
		AstNode* temp_node = &ast->nodes[ast->nodes_count++];

		AST_THROW_IF(ast->nodes_count > AST_MAX_NODES, "Max number of nodes reached");

		Term* next = term;
		while (next != NULL) {
			int temp_add = add;
			int r = ast_parse_term_mod(ast, temp_node, next, rule_name, &temp_add, &temp_src);
			if (r == AST_SUCCESS) {
				*src = temp_src;
				if (temp_add == 1) {
					*parent = temp_node;
					temp_node->next = NULL;
					parent = &temp_node->next;
					temp_node = &ast->nodes[ast->nodes_count++];
					AST_THROW_IF(ast->nodes_count > AST_MAX_NODES, "Max number of nodes reached");
				}
			} else {
				ast->nodes_count = temp_count;
				return r;
			}
			next = next->next;
		}

		ast->nodes_count--;
	}
	*src = temp_src;
	return AST_SUCCESS;
}

static int ast_parse_expression(Ast* ast, AstNode* node, Expression* expression, Str* rule_name, int add, Str* src, int one) {
	return ast_parse_term_list(ast, node, expression->term, rule_name, add, src, one);
}

static int ast_parse_rule(Ast* ast, AstNode* node, Rule* rule, Str* src) {
	if (rule->type == RULE_TYPE_VARIABLE_V || rule->type == RULE_TYPE_VARIABLE_I) {
		Expression* expression = rule->expression;
		int i = 0;
		while (expression != NULL) {
			Term* term = expression->term;
			Str temp_src = *src;
			Str text = str_read_str(&temp_src, &term->text);
			if (text.count > 0) {
				node->name = rule->name;
				node->type = rule->type == RULE_TYPE_VARIABLE_V ? ASTNODE_TYPE_VARIABLE_V : RULE_TYPE_VARIABLE_I;
				node->text = text;
				node->i = i;
				*src = temp_src;
				return AST_SUCCESS;
			}
			i++;
			expression = expression->next;
		}
	} else {
		Expression* next = rule->expression;
		while (next != NULL) {
			Str temp_src = *src;
			int r = ast_parse_expression(ast, node, next, &rule->name, 0, &temp_src, 0);
			if (r == AST_SUCCESS) {
				*src = temp_src;
				return AST_SUCCESS;
			} else if (r == AST_FAILURE) {
				return r;
			}
			next = next->next;
		}
	}

	return AST_END;
}

static int ast_parse_bnf(Ast* ast, AstNode* node, Bnf* bnf, Str* src) {
	node->type = ASTNODE_TYPE_PARENT;
	str_create_cstr(&node->name, "root", 0);

	node->child = NULL;
	AstNode** parent = &node->child;
	AstNode* temp_node = &ast->nodes[ast->nodes_count++];
	AST_THROW_IF(ast->nodes_count > AST_MAX_NODES, "Max number of nodes reached");

	str_read_space(src);
	Str temp_src = *src;
	int r = ast_parse_rule(ast, temp_node, &bnf->rules[0], &temp_src);
	if (r == AST_SUCCESS) {
		temp_node->name = bnf->rules[0].name;
		temp_node->type = ASTNODE_TYPE_PARENT;
		*parent = temp_node;
		temp_node->next = NULL;
		parent = &temp_node->next;
		*src = temp_src;
	} else {
		return r;
	}
	str_read_space(src);

	return AST_SUCCESS;
}

int ast_parse(Ast* ast, Bnf* bnf, const char* src) {
	str_create_cstr(&ast->src, src, 0);
	ast->error = "";
	ast->nodes = m_calloc(AST_MAX_NODES, sizeof(AstNode));
	AST_THROW_IF(ast->nodes == NULL, "Failed to allocate memory for nodes");
	ast->nodes_count = 0;
	ast->node = &ast->nodes[ast->nodes_count++];
	AST_THROW_IF(ast->nodes_count > AST_MAX_NODES, "Max number of nodes reached");

	return ast_parse_bnf(ast, ast->node, bnf, &ast->src);
}

void ast_delete(Ast* ast) {
	m_free(ast->nodes, AST_MAX_NODES * sizeof(AstNode));
}

AstNode* ast_get_node(AstNode* node, Str* name, int one) {
	if (one == 1) {
		if (node->type != ASTNODE_TYPE_LIST && str_cmp_str(name, &node->name)) {
			return node;
		}
	} else {
		AstNode* next = node->child;
		while (next != NULL) {
			if (next->type != ASTNODE_TYPE_LIST && str_cmp_str(name, &next->name)) {
				return next;
			}
			next = next->next;
		}
	}
	return NULL;
}

static AstNode* ast_get_node_list(AstNode* node) {
	AstNode* next = node->child;
	while (next != NULL) {
		if (next->type == ASTNODE_TYPE_LIST) {
			return next;
		}
		next = next->next;
	}
	return NULL;
}

static int ast_expression_match(Expression* expression, AstNode* node, int one) {
	Term* term = expression->term;
	while (term != NULL) {
		if (term->type == TERM_TYPE_RULE || term->type == TERM_TYPE_IDENTIFIER || term->type == TERM_TYPE_INT || term->type == TERM_TYPE_FLOAT) {
			AstNode* child = NULL;
			if (term->list == 1) {
				child = ast_get_node_list(node);
			} else {
				child = ast_get_node(node, &term->text, one);
			}
			if (child == NULL) {
				return 0;
			}
		}
		term = term->next;
	}
	return 1;
}

static Expression* ast_get_expression(AstNode* node, Expression* expression, int one) {
	Expression* next = expression;
	while (next != NULL) {
		if (ast_expression_match(next, node, one) == 1) {
			return next;
		}
		next = next->next;
	}
	return NULL;
}

static int ast_gen_rule(Ast* ast, AstNode* node, Rule* rule, int optional, Bnf* bnf, Str* dst);
static int ast_gen_expression(Ast* ast, AstNode* node, Expression* expression, int optional, Bnf* bnf, Str* dst, int one);

static int ast_gen_term(Ast* ast, AstNode* node, Term* term, Bnf* bnf, Str* dst) {
	if (term->list == 1) {
		AstNode* next = node->child;
		while (next != NULL) {
			Expression* expression = ast_get_expression(next, term->expression, term->one);
			ast_gen_expression(ast, next, expression, 0, bnf, dst, term->one);
			next = next->next;
		}
		return AST_SUCCESS;
	} else {
		switch (term->type)
		{
		case TERM_TYPE_RULE:
			return ast_gen_rule(ast, node, term->rule, term->optional, bnf, dst);
		case TERM_TYPE_LITERAL:
			str_add_str(dst, &term->text);
			return AST_SUCCESS;
		case TERM_TYPE_IDENTIFIER:
			if (node != NULL) {
				str_add_str(dst, &node->text);
				return AST_SUCCESS;
			}
			AST_ERROR("Identifier does not have node");
		case TERM_TYPE_INT:
			if (node != NULL) {
				str_add_str(dst, &node->text);
				return AST_SUCCESS;
			}
			AST_ERROR("Number does not have node");
		case TERM_TYPE_FLOAT:
			if (node != NULL) {
				str_add_str(dst, &node->text);
				return AST_SUCCESS;
			}
			AST_ERROR("Number does not have node");
		case TERM_TYPE_TAB:
			str_add_char(dst, '\t');
			return AST_SUCCESS;
		case TERM_TYPE_NEW_LINE:
			str_add_char(dst, '\n');
			return AST_SUCCESS;
		}
		AST_ERROR("Unknown term type");
	}
}

static int ast_gen_term_list(Ast* ast, AstNode* node, Term* term, Bnf* bnf, Str* dst, int one) {
	Term* next = term;
	while (next != NULL) {
		AstNode* child_node = NULL;
		if (next->type == TERM_TYPE_RULE || next->type == TERM_TYPE_IDENTIFIER || next->type == TERM_TYPE_INT || next->type == TERM_TYPE_FLOAT) {
			if (next->list == 1) {
				child_node = ast_get_node_list(node);
			} else {
				child_node = ast_get_node(node, &next->text, one);
			}
		}
		AST_THROW(ast_gen_term(ast, child_node, next, bnf, dst));
		next = next->next;
	}
	return AST_SUCCESS;
}

static int ast_gen_expression(Ast* ast, AstNode* node, Expression* expression, int optional, Bnf* bnf, Str* dst, int one) {
	return ast_gen_term_list(ast, node, expression->term, bnf, dst, one);
}

static int ast_gen_rule(Ast* ast, AstNode* node, Rule* rule, int optional, Bnf* bnf, Str* dst) {
	if (rule->type == RULE_TYPE_VARIABLE_V) {
		str_add_str(dst, &node->text);
	} else if (rule->type == RULE_TYPE_VARIABLE_I) {
		Expression* expression = rule->expression;
		int i = 0;
		while (expression != NULL) {
			if (node->i == i) {
				break;
			}
			i++;
			expression = expression->next;
		}
		if (expression != NULL) {
			str_add_str(dst, &expression->term->text);
		}
	} else {
		Expression* expression = ast_get_expression(node, rule->expression, 0);
		AST_THROW_IF(expression == NULL && optional == 0, "Expression not found");
		if (expression != NULL) {
			AST_THROW(ast_gen_expression(ast, node, expression, optional, bnf, dst, 0));
		}
	}
	return AST_SUCCESS;
}

static int ast_gen_node(Ast* ast, AstNode* node, Bnf* bnf, Str* dst) {
	Rule* rule = bnf_get_rule(bnf, &node->name);
	AST_THROW_IF(rule == NULL, "Rule not found");
	return ast_gen_rule(ast, node, rule, 0, bnf, dst);
}

int ast_gen(Ast* ast, Bnf* bnf, Str* dst) {
	return ast_gen_node(ast, ast->node->child, bnf, dst);
}

void ast_print_node(AstNode* node, int depth) {
	for (int i = 0; i < depth * 4; i++) {
		printf(" ");
	}

	str_print(&node->name);
	switch (node->type)
	{
	case ASTNODE_TYPE_PARENT:
		break;
	case ASTNODE_TYPE_LIST:
		printf(" list");
		break;
	case ASTNODE_TYPE_VARIABLE_V:
	case ASTNODE_TYPE_IDENTIFIER:
	case ASTNODE_TYPE_INT:
	case ASTNODE_TYPE_FLOAT:
		printf(" = ");
		str_print(&node->text);
		break;
	case ASTNODE_TYPE_VARIABLE_I:
		printf("(%i)", node->i);
		break;
	}

	printf("\n");
	AstNode* next = node->child;
	while (next != NULL) {
		ast_print_node(next, depth + 1);
		next = next->next;
	}
}

void ast_print(Ast* ast) {
	ast_print_node(ast->node, 0);
}