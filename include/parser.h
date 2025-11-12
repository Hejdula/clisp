#include "ast.h"
#include "env.h"

#ifndef PARSER_H
#define PARSER_H

int parse_list(astnode **out_node, const char **tokens, int *curr_tok);
int parse_expr(astnode **out_node, const char **tokens, int *curr_tok);

#endif