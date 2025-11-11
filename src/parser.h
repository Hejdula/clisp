#include "ast.h"
#include "env.h"

#ifndef PARSER_H
#define PARSER_H

astnode *parse_tree(env*);
astnode *parse_expr(env*);

#endif