#include "ast.h"
#include "err.h"

#ifndef OPERATORS_H
#define OPERATORS_H

#define OPERATOR(fname) err_t fname(astnode *list_node, astnode **result_node, env* env)

struct operator_entry {
  char *symbol;
  OPERATOR((*func));
};

/* operators */
OPERATOR(oper_add);
OPERATOR(oper_sub);
OPERATOR(oper_mul);
OPERATOR(oper_div);
OPERATOR(oper_inc);
OPERATOR(oper_dec);
/* relational */
OPERATOR(oper_noneql);
OPERATOR(oper_eql);
OPERATOR(oper_lwr);
OPERATOR(oper_grt);
OPERATOR(oper_lwreql);
OPERATOR(oper_grteql);
OPERATOR(oper_max);
OPERATOR(oper_min);
/* functions */
OPERATOR(oper_quote);
OPERATOR(oper_set);

extern struct operator_entry operators[];
extern int oper_count;
#endif