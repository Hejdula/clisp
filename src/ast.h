#include "err.h"

enum node_type {
  LIST,
  SYMBOL,
  NUMBER,
};

/**
 * @brief An abstract syntax tree node representing either LIST, SYMBOL or
 * NUMBER.
 */
typedef struct ASTnode {
  enum node_type type;
  union {
    int value;
    char *symbol;
    struct {
      struct ASTnode **children;
      int count;
    } list;
  } as;
} astnode;

astnode* get_astnode(enum node_type type, char* value);
err_t add_child_node(astnode* parent, astnode* child);
astnode* eval_node(astnode* node);
void free_node(astnode* node);
