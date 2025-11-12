#include "ast.h"

struct operator_entry{
  char* symbol;
  int (*func)(astnode*);
};
extern struct operator_entry operators[

];