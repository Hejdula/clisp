#include "ast.h"
#include "err.h"
#include "macros.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates and returns empty list node,
 *
 * @return astnode* or NULL if memory could not be allocated
 */
astnode *get_list_node() {
  astnode *nptr = calloc(1, sizeof(astnode));
  RETURN_NULL_IF(!nptr);
  nptr->type = LIST;
  return nptr;
}

/**
 * @brief Allocates and returns a symbol node
 *
 * @param symbol for the node
 * @return astnode* or NULL if could not allocate memory
 */
astnode *get_symbol_node(const char *symbol) {
  // /* sanity check */
  // RETURN_NULL_IF(!symbol);
  size_t len = strlen(symbol);
  // RETURN_NULL_IF(!len);

  astnode *nptr = malloc(sizeof(astnode));
  RETURN_NULL_IF(!nptr);
  nptr->type = SYMBOL;
  nptr->as.symbol = malloc(len + 1);
  if (!nptr->as.symbol) {
    free(nptr);
    return NULL;
  }
  memcpy(nptr->as.symbol, symbol, len + 1);

  return nptr;
}

/**
 * @brief Get the number node object
 *
 * @param value integer value for the node
 * @return astnode* or NULL if memory could not be allocated
 */
astnode *get_number_node(int value) {
  astnode *nptr = malloc(sizeof(astnode));
  RETURN_NULL_IF(!nptr);
  nptr->type = NUMBER;
  nptr->as.value = value;
  return nptr;
}

/**
 * @brief appends given node to parents children array
 *
 * @param parent LIST type node
 * @param child Node to add
 * @return err_t
 */
err_t add_child_node(astnode *parent, astnode *child) {
  /* sanity checks */
  RETURN_ERR_IF(!parent || !child || parent->type != LIST, ERR_INTERNAL);

  astnode **tmp = realloc(parent->as.list.children,
                          (parent->as.list.count + 1) * sizeof(astnode *));
  RETURN_ERR_IF(!tmp, ERR_OUT_OF_MEMORY);

  parent->as.list.children = tmp;
  parent->as.list.children[parent->as.list.count] = child;
  parent->as.list.count++;
  return ERR_NO_ERROR;
}

astnode *eval_node(astnode *node);

/**
 * @brief Frees the node memory and recursively all children nodes
 *
 * @param node to free
 */
void free_node(astnode *node) {
  if (!node)
    return;
  if (node->type == SYMBOL) {
    free(node->as.symbol);
  }
  if (node->type == LIST && node->as.list.children) {
    for (int i = 0; i < node->as.list.count; i++) {
      free_node(node->as.list.children[i]);
    }
    free(node->as.list.children);
  }
  free(node);
}

void print_node(astnode *node) { 
  if (!node) {
    fputs("NIL", stdout);
    return;
  }

  switch (node->type) {
  case NUMBER:
    printf("%d", node->as.value);
    break;
  case SYMBOL:
    if (node->as.symbol) {
      fputs(node->as.symbol, stdout);
    } else {
      fputs("()", stdout);
    }
    break;
  case LIST: {
    fputc('(', stdout);
    for (int i = 0; i < node->as.list.count; ++i) {
      if (i) fputc(' ', stdout);
      print_node(node->as.list.children[i]);
    }
    fputc(')', stdout);
    break;
  }
  default:
    fputs("()", stdout);
    break;
  }
}
