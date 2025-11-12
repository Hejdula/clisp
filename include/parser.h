#include "ast.h"
#include "err.h"

#ifndef PARSER_H
#define PARSER_H

err_t parse_list(astnode **out_node, const char **tokens, int *curr_tok);
err_t parse_expr(astnode **out_node, const char **tokens, int *curr_tok);

#endif