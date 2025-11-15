#include "operators.h"
#include "ast.h"
#include "env.h"
#include "err.h"
#include "macros.h"
#include <stdlib.h>

err_t oper_add(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err;
  int i, sum = 0;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    RETURN_ERR_IF(temp_node->type != NUMBER, ERR_SYNTAX_ERROR);
    sum += temp_node->as.value;
    free_node_if_temporary(temp_node);
  }

  *result_node = get_number_node(sum);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
}

err_t oper_sub(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err;
  int i, sum = 0;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    RETURN_ERR_IF(temp_node->type != NUMBER, ERR_SYNTAX_ERROR);
    sum += (i == 1 ? temp_node->as.value : -temp_node->as.value);
    free_node_if_temporary(temp_node);
  }

  *result_node = get_number_node(sum);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
}

err_t oper_mul(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err;
  int i, prod = 1;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    RETURN_ERR_IF(temp_node->type != NUMBER, ERR_SYNTAX_ERROR);
    prod *= temp_node->as.value;
    free_node_if_temporary(temp_node);
  }

  *result_node = get_number_node(prod);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
}

err_t oper_div(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err;
  int i, sum = 0;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    RETURN_ERR_IF(temp_node->type != NUMBER, ERR_SYNTAX_ERROR);
    sum = (i == 1 ? temp_node->as.value : sum / temp_node->as.value);
    free_node_if_temporary(temp_node);
  }

  *result_node = get_number_node(sum);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
}

err_t oper_set(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval = ERR_NO_ERROR;
  astnode *var_node = NULL, *value_node = NULL, *value_node_copy = NULL;

  /* get variable node to set */
  if (list_node->as.list.children[1]->type == SYMBOL &&
      !exists_var(list_node->as.list.children[1]->as.symbol, env)) {
    err = add_empty_var(list_node->as.list.children[1]->as.symbol, env);
  };

  err = eval_node(list_node->as.list.children[1], &var_node, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(var_node->origin != VARIABLE, cleanup, ERR_SYNTAX_ERROR);

  /* obtain value to asign */
  err = eval_node(list_node->as.list.children[2], &value_node, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(value_node->type == SYMBOL, cleanup, ERR_SYNTAX_ERROR);

  /* make node copy with origin VARIABLE */
  err = make_variable_deep_copy(value_node, &value_node_copy);
  CLEANUP_WITH_ERR_IF(err, cleanup, err);
  
  free_node_content(var_node);
  *var_node = *value_node_copy;
  *result_node = var_node;
  
  free(value_node_copy);
cleanup:
  free_node_if_temporary(var_node);
  free_node_if_temporary(value_node);
  return retval;
}

struct operator_entry operators[] = {
    {"+", oper_add}, {"-", oper_sub},   {"*", oper_mul},
    {"/", oper_div}, {"SET", oper_set},
};

int oper_count = sizeof(operators) / sizeof(operators[0]);