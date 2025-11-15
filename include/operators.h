#include "ast.h"
#include "err.h"

#ifndef OPERATORS_H
#define OPERATORS_H

#define OPERATOR(fname) err_t fname(astnode *list_node, astnode **result_node, env* env)

struct operator_entry {
  char *symbol;
  OPERATOR((*func));
};


OPERATOR(oper_add);
OPERATOR(oper_sub);
OPERATOR(oper_mul);
OPERATOR(oper_div);
OPERATOR(oper_inc);
OPERATOR(oper_dec);
OPERATOR(oper_set);
OPERATOR(oper_quote);

extern struct operator_entry operators[];
extern int oper_count;
#endif