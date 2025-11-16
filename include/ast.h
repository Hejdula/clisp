#include "err.h"

#ifndef AST_H
#define AST_H

typedef struct Env env;

enum node_type {
  BOOLEAN,
  NUMBER,
  SYMBOL,
  LIST,
};

enum node_origin {
  AST,
  VARIABLE,
  TEMPORARY,
};

/**
 * @brief An abstract syntax tree node representing either LIST, SYMBOL or
 * NUMBER.
 */
typedef struct ASTnode {
  enum node_type type;
  enum node_origin origin;
  union {
    int value;
    char *symbol;
    struct {
      struct ASTnode **children;
      int count;
      int return_type;
    } list;
  } as;
} astnode;

astnode *get_list_node();
astnode *get_symbol_node(const char *symbol);
astnode *get_bool_node(int truthy);
astnode *get_number_node(int value);
err_t add_child_node(astnode *parent, astnode *child);
err_t eval_node(astnode *node, astnode **out_node, env *env);
err_t make_deep_copy(astnode *original_node, astnode **new_node, enum node_origin origin);
void print_node(astnode *node);
void free_node(astnode *node);
void free_node_content(astnode *node);
void free_temp_node_parts(astnode *node);

#endif