#include "ast.h"
#include "env.h"
#include "err.h"
#include "macros.h"
#include "operators.h"
#include <stddef.h>
#include <stdio.h>
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
  nptr->origin = UNSET;
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
  size_t len = strlen(symbol);

  astnode *nptr = calloc(1, sizeof(astnode));
  RETURN_NULL_IF(!nptr);
  nptr->origin = UNSET;
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
 * @brief Allocates and returns a boolean node
 *
 * @param truthy boolean the node should represent
 * @return astnode* or NULL if could not allocate memory
 */
astnode *get_bool_node(int truthy) {
  astnode *nptr = calloc(1, sizeof(astnode));
  RETURN_NULL_IF(!nptr);
  nptr->origin = UNSET;
  nptr->type = BOOLEAN;
  nptr->as.value = truthy ? 1 : 0;
  return nptr;
}

/**
 * @brief Allocates and returns a number node representing the value
 *
 * @param value integer value for the node
 * @return astnode* or NULL if memory could not be allocated
 */
astnode *get_number_node(int value) {
  astnode *nptr = malloc(sizeof(astnode));
  RETURN_NULL_IF(!nptr);
  nptr->origin = UNSET;
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
  /* sanity check */
  RETURN_ERR_IF(!parent || !child || parent->type != LIST, ERR_INTERNAL);

  astnode **tmp = realloc(parent->as.list.children,
                          (parent->as.list.count + 1) * sizeof(astnode *));
  RETURN_ERR_IF(!tmp, ERR_OUT_OF_MEMORY);

  parent->as.list.children = tmp;
  parent->as.list.children[parent->as.list.count] = child;
  parent->as.list.count++;
  return ERR_NO_ERROR;
}

/**
 * @brief Base function for node evaluation
 * puts the result node into out_node argument
 *
 * @param node to evaluate
 * @param out_node out param, evaluation result, NULL on failure
 * @param env Environment in which to evaluate
 * @return err_t
 */
err_t eval_node(astnode *node, astnode **out_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!node || !env, ERR_INTERNAL);

  int i, err;

  switch (node->type) {
  case BOOLEAN:
    *out_node = node;
    break;
  case NUMBER:
    *out_node = node;
    break;
  case SYMBOL:
    *out_node = get_var(node->as.symbol, env);
    RETURN_ERR_IF(!*out_node, ERR_RUNTIME_UNKNOWN_VAR);
    break;
  case LIST:
    /* return NIL if empty list  */
    if (!node->as.list.count) {
      *out_node = get_bool_node(0);
      RETURN_ERR_IF(!*out_node, ERR_OUT_OF_MEMORY);
      (*out_node)->origin = TEMPORARY;
      break;
    }
    RETURN_ERR_IF(!node->as.list.children, ERR_INTERNAL);
    /* on list evaluation first child has to be function operator represented as
     * symbol */
    RETURN_ERR_IF(node->as.list.children[0]->type != SYMBOL, ERR_SYNTAX_ERROR);

    /* if known function operator, execute it */
    err_t (*func)(astnode *node, astnode **out_node, struct Env *env) = NULL;
    // printf("%s\n", node->as.list.children[0]->as.symbol);
    for (i = 0; i < oper_count; i++) {
      if (!strcmp(operators[i].symbol, node->as.list.children[0]->as.symbol)) {
        func = operators[i].func;
        break;
      }
    }
    RETURN_ERR_IF(!func, ERR_SYNTAX_ERROR);

    err = func(node, out_node, env);
    RETURN_ERR_IF(err, err);

    break;
  }
  return ERR_NO_ERROR;
}
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
                     enum node_origin origin) {
  err_t retval = ERR_NO_ERROR;
  RETURN_ERR_IF(!original_node || !new_node, ERR_INTERNAL);

  astnode *child_clone = NULL, *copy = NULL;
  switch (original_node->type) {
  case NUMBER:
    copy = get_number_node(original_node->as.value);
    CLEANUP_WITH_ERR_IF(!copy, fail_cleanup, ERR_OUT_OF_MEMORY);
    break;
  case BOOLEAN:
    copy = get_bool_node(original_node->as.value);
    CLEANUP_WITH_ERR_IF(!copy, fail_cleanup, ERR_OUT_OF_MEMORY);
    break;
  case SYMBOL:
    copy = get_symbol_node(original_node->as.symbol);
    CLEANUP_WITH_ERR_IF(!copy, fail_cleanup, ERR_OUT_OF_MEMORY);
    break;
  case LIST: {
    copy = get_list_node();
    CLEANUP_WITH_ERR_IF(!copy, fail_cleanup, ERR_OUT_OF_MEMORY);
    /* recursively make copy and add all childen to resulting node */
    for (int i = 0; i < original_node->as.list.count; ++i) {
      retval = make_deep_copy(original_node->as.list.children[i], &child_clone,
                              origin);
      CLEANUP_WITH_ERR_IF(retval, fail_cleanup, retval);
      retval = add_child_node(copy, child_clone);
      CLEANUP_WITH_ERR_IF(retval, fail_cleanup, retval);
    }
    break;
  }
  /* Node should alwais have a valid Type */
  default:
    return ERR_INTERNAL;
  }

  copy->origin = origin;
  *new_node = copy;
  return ERR_NO_ERROR;

fail_cleanup:
  free_node(child_clone);
  free_node(copy);
  return retval;
}

/**
 * @brief Frees any memory asociated with the node and in the AST tree below it
 * but not the node itself.
 *
 * @param node to free
 */
void free_node_content(astnode *node) {
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
}

/**
 * @brief Frees the node memory and recursively all children nodes
 *
 * @param node to free
 */
void free_node(astnode *node) {
  free_node_content(node);
  free(node);
}

/**
 * @brief Recursively free the whole AST tree, but leave out non-temporary nodes
 * it expects non-temporary astnodes to not have any temporary subnodes,
 * thus does not check further down the tree on non-temporary nodes,
 *
 * @param node to free
 */
void free_temp_node_parts(astnode *node) {
  if (!node)
    return;
  if (node->origin != TEMPORARY)
    return;

  switch (node->type) {
  case BOOLEAN:
  case NUMBER:
    free(node);
    return;
  case SYMBOL:
    free(node->as.symbol);
    free(node);
    return;
  case LIST:
    for (int i = 0; i < node->as.list.count; i++) {
      free_temp_node_parts(node->as.list.children[i]);
      node->as.list.children[i] = NULL;
    }
    free(node->as.list.children);
    node->as.list.children = NULL;
    free(node);
    return;
  }
};

/**
 * @brief Prints the AST node to standard output in Lisp-like format.
 *
 * The output format is:
 * - Numbers are printed as integers (e.g., 42).
 * - Symbols are printed as their string names (e.g., add).
 * - Lists are printed as parentheses containing space-separated child nodes
 * (e.g., (add 1 2)).
 * - NULL nodes are printed as NIL.
 */
void print_node(astnode *node) {
  if (!node) {
    fputs("NIL", stdout);
    return;
  }

  switch (node->type) {
  case NUMBER:
    printf("%d", node->as.value);
    break;
  case BOOLEAN:
    fputs(node->as.value ? "T" : "NIL", stdout);
    break;
  case SYMBOL:
    if (node->as.symbol) {
      fputs(node->as.symbol, stdout);
    } else {
      fputs("??", stdout);
    }
    break;
  case LIST: {
    fputc('(', stdout);
    for (int i = 0; i < node->as.list.count; ++i) {
      if (i)
        fputc(' ', stdout);
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
