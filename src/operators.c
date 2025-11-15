#include "operators.h"
#include "ast.h"
#include "env.h"
#include "err.h"
#include "macros.h"
#include <stdlib.h>
#include <string.h>

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

err_t oper_inc(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval;
  astnode *var_node = NULL, *value_node = NULL;

  err = eval_node(list_node->as.list.children[1], &var_node, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(var_node->origin != VARIABLE, cleanup,
                      ERR_NOT_A_VARIABLE);
  CLEANUP_WITH_ERR_IF(var_node->type != NUMBER, cleanup, ERR_SYNTAX_ERROR);

  err = eval_node(list_node->as.list.children[2], &value_node, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(value_node->type != NUMBER, cleanup, err);

  var_node->as.value += value_node->as.value;
  *result_node = var_node;

  retval = ERR_NO_ERROR;

cleanup:
  free_node_if_temporary(var_node);
  free_node_if_temporary(value_node);
  return retval;
}

err_t oper_dec(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval;
  astnode *var_node = NULL, *value_node = NULL;

  err = eval_node(list_node->as.list.children[1], &var_node, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(var_node->origin != VARIABLE, cleanup,
                      ERR_NOT_A_VARIABLE);
  CLEANUP_WITH_ERR_IF(var_node->type != NUMBER, cleanup, ERR_SYNTAX_ERROR);

  err = eval_node(list_node->as.list.children[2], &value_node, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(value_node->type != NUMBER, cleanup, err);

  var_node->as.value -= value_node->as.value;
  *result_node = var_node;

  retval = ERR_NO_ERROR;

cleanup:
  free_node_if_temporary(var_node);
  free_node_if_temporary(value_node);
  return retval;
}

err_t oper_eql(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval = ERR_NO_ERROR;
  int ref_val, all_equal = 1;
  astnode *temp;

  err = eval_node(list_node->as.list.children[1], &temp, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(temp->type != NUMBER, fail_cleanup, ERR_SYNTAX_ERROR);
  ref_val = temp->as.value;
  free_node_if_temporary(temp);

  for (int i = 2; i < list_node->as.list.count; i++) {
    err = eval_node(list_node->as.list.children[i], &temp, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp->type != NUMBER, fail_cleanup, ERR_SYNTAX_ERROR);
    if (ref_val != temp->as.value) {
      all_equal = 0;
      break;
    }
    free_node_if_temporary(temp);
  }

  *result_node = get_bool_node(all_equal);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return retval;
fail_cleanup:
  free_node_if_temporary(temp);
  return retval;
};

err_t oper_noneql(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval = ERR_NO_ERROR;
  int all_non_equal = 1;
  int *values = malloc(sizeof(int) * list_node->as.list.count);
  astnode *temp;

  for (int i = 1; i < list_node->as.list.count; i++) {
    err = eval_node(list_node->as.list.children[i], &temp, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp->type != NUMBER, fail_cleanup, ERR_SYNTAX_ERROR);
    for (int j = 0; j < (i - 1); j++) {
      if (values[j] == temp->as.value) {
        all_non_equal = 0;
        break;
      }
    }
    values[i - 1] = temp->as.value;
    free_node_if_temporary(temp);
  }
  free(values);

  *result_node = get_bool_node(all_non_equal);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return retval;
fail_cleanup:
  free(values);
  free_node_if_temporary(temp);
  return retval;
};

err_t oper_grt_lwr(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval = ERR_NO_ERROR;
  int all_true = 1, prev_val;
  astnode *temp;

  for (int i = 1; i < list_node->as.list.count; i++) {
    err = eval_node(list_node->as.list.children[i], &temp, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp->type != NUMBER, fail_cleanup, ERR_SYNTAX_ERROR);
    if (i == 1)
      prev_val = temp->as.value;

    if (!strcmp(list_node->as.list.children[0]->as.symbol, "<")) {
      if(!(prev_val < temp->as.value) && i != 1){
        all_true = 0;
        break;
      }
    } else if (!strcmp(list_node->as.list.children[0]->as.symbol, ">")) {
      if(!(prev_val > temp->as.value)&& i != 1){
        all_true = 0;
        break;
      }
    } else if (!strcmp(list_node->as.list.children[0]->as.symbol, ">=")) {
      if(!(prev_val >= temp->as.value)){
        all_true = 0;
        break;
      }
    } else if (!strcmp(list_node->as.list.children[0]->as.symbol, "<=")) {
      if(!(prev_val <= temp->as.value)){
        all_true = 0;
        break;
      }
    } else {
      retval = ERR_SYNTAX_ERROR;
      goto fail_cleanup;
    }
    prev_val = temp->as.value;
    free_node_if_temporary(temp);
  }

  *result_node = get_bool_node(all_true);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return retval;
fail_cleanup:
  free_node_if_temporary(temp);
  return retval;
};

// err_t oper_lwr(astnode *list_node, astnode **result_node, env *env) {};
// err_t oper_grt(astnode *list_node, astnode **result_node, env *env) {};
// err_t oper_lwreql(astnode *list_node, astnode **result_node, env *env) {};
// err_t oper_grteql(astnode *list_node, astnode **result_node, env *env) {};
// err_t oper_max(astnode *list_node, astnode **result_node, env *env) {};
// err_t oper_min(astnode *list_node, astnode **result_node, env *env) {};

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
  CLEANUP_WITH_ERR_IF(var_node->origin != VARIABLE, cleanup,
                      ERR_NOT_A_VARIABLE);

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

err_t oper_quote(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  *result_node = list_node->as.list.children[1];
  return ERR_NO_ERROR;
}

struct operator_entry operators[] = {
    /* opers */
    {"+", oper_add},
    {"-", oper_sub},
    {"*", oper_mul},
    {"/", oper_div},
    {"INC", oper_inc},
    {"DEC", oper_dec},
    /* relational */
    {"=", oper_eql},
    {"/=", oper_noneql},
    {"<", oper_grt_lwr},
    {">", oper_grt_lwr},
    {"<=", oper_grt_lwr},
    {">=", oper_grt_lwr},
    // {"MAX", oper_max},
    // {"MIN", oper_min},
    /* func */
    {"QUOTE", oper_quote},
    {"SET", oper_set},
};

int oper_count = sizeof(operators) / sizeof(operators[0]);