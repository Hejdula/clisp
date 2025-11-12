#include "ast.h"

struct operator_entry {
  char *symbol;
  astnode *(*func)(astnode *);
};
extern struct operator_entry operators[

];