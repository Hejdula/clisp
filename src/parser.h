#include "ast.h"
#include "env.h"

#ifndef PARSER_H
#define PARSER_H

astnode *parse_list(const char** tokens, int *curr_tok);
astnode *parse_expr(const char** tokens, int *curr_tok);

#endif