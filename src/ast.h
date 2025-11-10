#include "err.h"

typedef struct ASTnode astnode;

enum node_type {
  LIST,
  SYMBOL,
  NUMBER,
};

astnode* get_astnode(enum node_type type, char* value);
err_t add_child_node(astnode* parent, astnode* child);
astnode* eval_node(astnode* node);
void free_node(astnode* node);
