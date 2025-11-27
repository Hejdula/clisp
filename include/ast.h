#include "err.h"

#ifndef AST_H
#define AST_H

typedef struct Env env;

/**
 * @brief Type of the AST node, to distinquish what the node represents
 */
enum node_type {
  BOOLEAN,
  NUMBER,
  SYMBOL,
  LIST,
};

/**
 * @brief Marker to distinguish with what purpose were the nodes created
 * This is necessary for managing memory and freeing nodes passed as temporary results
 */
enum node_origin {
  UNSET,
  AST,
  VARIABLE,
  TEMPORARY,
};

/**
 * @brief An abstract syntax tree node representing either LIST, SYMBOL, BOOLEAN or a
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
    } list;
  } as;
} astnode;


/**
 * @brief Allocates and returns empty list node.
 *
 * @return astnode* or NULL if memory could not be allocated
 */
astnode *get_list_node();

/**
 * @brief Allocates and returns a symbol node
 *
 * @param symbol for the node
 * @return astnode* or NULL if could not allocate memory
 */
astnode *get_symbol_node(const char *symbol);

/**
 * @brief Allocates and returns a boolean node
 *
 * @param truthy boolean the node should represent
 * @return astnode* or NULL if could not allocate memory
 */
astnode *get_bool_node(int truthy);

/**
 * @brief Allocates and returns a number node representing the value
 *
 * @param value integer value for the node
 * @return astnode* or NULL if memory could not be allocated
 */
astnode *get_number_node(int value);

/**
 * @brief appends given node to parents children array
 *
 * @param parent LIST type node
 * @param child Node to add
 * @return err_t
 */
err_t add_child_node(astnode *parent, astnode *child);

/**
 * @brief Base function for node evaluation
 * puts the result node into out_node argument
 *
 * @param node to evaluate
 * @param out_node out param, evaluation result, NULL on failure
 * @param env Environment in which to evaluate
 * @return err_t
 */
err_t eval_node(astnode *node, astnode **out_node, env *env);

/**
 * @brief Makes a deep copy recursively with all children and marks their origin
 * with given value
 *
 * @param original_node node to copy
 * @param new_node out param, copy of the node
 * @param origin enum to mark the nodes.
 * @return err_t
 */
err_t make_deep_copy(astnode *original_node, astnode **new_node,
                     enum node_origin origin);

/**
 * @brief Prints the AST node to standard output in Lisp-like format.
 *
 * The output format is:
 * - Numbers are printed as integers (e.g., 42).
 * - Symbols are printed as their string names (e.g., add).
 * - Lists are printed as parentheses containing space-separated child nodes
 *   (e.g., (add 1 2)).
 * - NULL nodes are printed as NIL.
 */
void print_node(astnode *node);

/**
 * @brief Frees the node memory and recursively all children nodes
 *
 * @param node to free
 */
void free_node(astnode *node);

/**
 * @brief Frees any memory asociated with the node and in the AST tree below it
 * but not the node itself.
 *
 * @param node to free
 */
void free_node_content(astnode *node);

/**
 * @brief Recursively free the whole AST tree, but leave out non-temporary nodes
 * it expects non-temporary astnodes to not have any temporary subnodes,
 * thus does not check further down the tree on non-temporary nodes,
 *
 * @param node to free
 */
void free_temp_node_parts(astnode *node);

#endif