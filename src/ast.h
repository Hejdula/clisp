#include "err.h"

#ifndef AST_H
#define AST_H

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

astnode *get_list_node();
astnode *get_symbol_node(const char *symbol);
astnode *get_number_node(int value);
err_t add_child_node(astnode* parent, astnode* child);
astnode* eval_node(astnode* node);
void print_node(astnode *node);
void free_node(astnode* node);

#endif