#include "ast.h"

typedef struct ASTnode {
  enum Ntype type;
  char* value;
  int child_count;
  struct ASTnode *children;
} astnode;

astnode * create_astnode(enum Ntype type, char* value);