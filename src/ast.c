#include "ast.h"

/**
 * @brief An abstract syntax tree node of Lisp language
 * 
 */
typedef struct ASTnode {
  enum node_type type;
  char* value;
  int child_count;
  struct ASTnode *children;
} astnode;

astnode * create_astnode(enum node_type type, char* value);
int add_child_node(astnode* parent, astnode* child);
char* eval_node(astnode* node);
