#include "ast.h"
#include "err.h"

struct operator_entry {
  char *symbol;
  err_t (*func)(astnode * node, astnode ** result_node, env *env);
};

err_t oper_add(astnode *list_node, astnode **result_node, env* env);

extern struct operator_entry operators[];
extern int oper_count;