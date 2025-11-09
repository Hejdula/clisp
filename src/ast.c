#include "ast.h"
#include "err.h"
#include "macros.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief An abstract syntax tree node
 *
 */
typedef struct ASTnode {
  enum node_type type;
  char *value;
  int child_count;
  struct ASTnode **children;
} astnode;

/**
 * @brief Allocates memory for the node and value if provided,
 * fills the astnode struct with given data
 *
 * @param type node_type enum
 * @param value can be null, then no space is allocated
 * @return pointer to the created astnode, NULL if memory could not be allocated
 */
astnode *get_astnode(enum node_type type, char *value) {
  astnode *nptr = calloc(1, sizeof(astnode));
  if (!nptr)
    return NULL;
  if (value) {
    nptr->value = malloc(strlen(value));
    if (!nptr->value) {
      free(nptr);
      return NULL;
    }
    memcpy(nptr->value, value, strlen(value));
  }
  nptr->type = type;
  return nptr;
};

/**
 * @brief appends given node to parents children array
 *
 * @param parent
 * @param child
 * @return
 */
err_t add_child_node(astnode *parent, astnode *child) {
  /* sanity checks */
  RETURN_ERR_IF(!parent || !child, ERR_INTERNAL);

  if (!parent->children) {
    parent->children = malloc(0);
    RETURN_ERR_IF(!parent->children, ERR_OUT_OF_MEMORY);
  }
  parent->child_count++;
  parent->children =
      realloc(parent->children, parent->child_count * sizeof(astnode *));
  RETURN_ERR_IF(!parent->children, ERR_OUT_OF_MEMORY);

  parent->children[parent->child_count - 1] = child;
  return ERR_NO_ERROR;
}

astnode *eval_node(astnode *node);

/**
 * @brief Frees the node memory with value and children array
 *
 * @param node to free
 */
void free_node(astnode *node) {

};
