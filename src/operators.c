#include "operators.h"
#include "ast.h"
#include "err.h"
#include "macros.h"
#include "stdio.h"

err_t oper_add(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST ||
                    list_node->as.list.count < 3,
                ERR_INTERNAL);

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
  RETURN_ERR_IF(!list_node || list_node->type != LIST ||
                    list_node->as.list.count < 3,
                ERR_INTERNAL);

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
  RETURN_ERR_IF(!list_node || list_node->type != LIST ||
                    list_node->as.list.count < 3,
                ERR_INTERNAL);

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
  RETURN_ERR_IF(!list_node || list_node->type != LIST ||
                    list_node->as.list.count < 3,
                ERR_INTERNAL);

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

struct operator_entry operators[] = {
    {"+", oper_add},
    {"-", oper_sub},
    {"*", oper_mul},
    {"/", oper_div},
};

int oper_count = sizeof(operators) / sizeof(operators[0]);