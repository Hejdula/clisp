#include "operators.h"
#include "ast.h"
#include "env.h"
#include "err.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Evaluates and sums the arguments and returns a new NUMBER node.
 * All arguments must evaluate to NUMBER nodes or a syntax error is returned.
 * @param list_node List node containing the operator and arguments
 * @param result_node out param pointer to the result NUMBER node with TEMPORARY
 * origin, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_add(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err, retval = ERR_NO_ERROR;
  int i, sum = 0;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp_node->type != NUMBER, fail_cleanup,
                        ERR_SYNTAX_ERROR);
    sum += temp_node->as.value;
    free_temp_node_parts(temp_node);
  }

  *result_node = get_number_node(sum);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(temp_node);
  return retval;
}

/**
 * @brief Evaluates and subtracts all other arguments after the first one and
 * returns a new NUMBER node. All arguments must evaluate to NUMBER nodes or a
 * syntax error is returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result NUMBER node with TEMPORARY
 * origin, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_sub(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err, retval = ERR_NO_ERROR;
  int i, sum = 0;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp_node->type != NUMBER, fail_cleanup,
                        ERR_SYNTAX_ERROR);
    sum += (i == 1 ? temp_node->as.value : -temp_node->as.value);
    free_temp_node_parts(temp_node);
  }

  *result_node = get_number_node(sum);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(temp_node);
  return retval;
}

/**
 * @brief Evaluates and multiplies the arguments with eachother and returns a
 * new NUMBER node. All arguments must evaluate to NUMBER nodes or a syntax
 * error is returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result NUMBER node with TEMPORARY
 * origin, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_mul(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err, retval = ERR_NO_ERROR;
  int i, prod = 1;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp_node->type != NUMBER, fail_cleanup,
                        ERR_SYNTAX_ERROR);
    prod *= temp_node->as.value;
    free_temp_node_parts(temp_node);
  }

  *result_node = get_number_node(prod);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(temp_node);
  return retval;
}

/**
 * @brief Evaluates and divides first by all other arguments and returns a new
 * NUMBER node. All arguments must evaluate to NUMBER nodes or a syntax error is
 * returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result NUMBER node with TEMPORARY
 * origin, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_div(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);

  err_t err, retval = ERR_NO_ERROR;
  int i, res = 0;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp_node->type != NUMBER, fail_cleanup,
                        ERR_SYNTAX_ERROR);
    CLEANUP_WITH_ERR_IF(i != 1 && temp_node->as.value == 0, fail_cleanup,
                        ERR_ZERO_DIVISON);
    res = (i == 1 ? temp_node->as.value : res / temp_node->as.value);
    free_temp_node_parts(temp_node);
  }

  *result_node = get_number_node(res);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(temp_node);
  return retval;
}

/**
 * @brief Increments a variable by a value and returns the updated variable
 * node. The first argument must be a variable node, the second a NUMBER node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the updated variable node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_inc(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
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
  free_temp_node_parts(var_node);
  free_temp_node_parts(value_node);
  return retval;
}

/**
 * @brief Decrements a variable by a value and returns the updated variable
 * node. The first argument must be a variable node, the second a NUMBER node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the updated variable node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_dec(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
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
  free_temp_node_parts(var_node);
  free_temp_node_parts(value_node);
  return retval;
}

/**
 * @brief Checks if all arguments are equal and returns a BOOLEAN node.
 * All arguments must evaluate to NUMBER nodes or a syntax error is returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result BOOLEAN node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_eql(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
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
  free_temp_node_parts(temp);

  for (int i = 2; i < list_node->as.list.count; i++) {
    err = eval_node(list_node->as.list.children[i], &temp, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp->type != NUMBER, fail_cleanup, ERR_SYNTAX_ERROR);
    if (ref_val != temp->as.value) {
      all_equal = 0;
      break;
    }
    free_temp_node_parts(temp);
  }

  *result_node = get_bool_node(all_equal);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return retval;
fail_cleanup:
  free_temp_node_parts(temp);
  return retval;
};

/**
 * @brief Checks if all arguments are non-equal and returns a BOOLEAN node.
 * All arguments must evaluate to NUMBER nodes or a syntax error is returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result BOOLEAN node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_noneql(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
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
    free_temp_node_parts(temp);
  }
  free(values);

  *result_node = get_bool_node(all_non_equal);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return retval;
fail_cleanup:
  free(values);
  free_temp_node_parts(temp);
  return retval;
};

/**
 * @brief Compares arguments according to the relational operator (<, >, <=, >=)
 * and returns a BOOLEAN node. All arguments must evaluate to NUMBER nodes or a
 * syntax error is returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result BOOLEAN node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_grt_lwr(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
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
      if (!(prev_val < temp->as.value) && i != 1) {
        all_true = 0;
        break;
      }
    } else if (!strcmp(list_node->as.list.children[0]->as.symbol, ">")) {
      if (!(prev_val > temp->as.value) && i != 1) {
        all_true = 0;
        break;
      }
    } else if (!strcmp(list_node->as.list.children[0]->as.symbol, ">=")) {
      if (!(prev_val >= temp->as.value)) {
        all_true = 0;
        break;
      }
    } else if (!strcmp(list_node->as.list.children[0]->as.symbol, "<=")) {
      if (!(prev_val <= temp->as.value)) {
        all_true = 0;
        break;
      }
    } else {
      retval = ERR_INTERNAL;
      goto fail_cleanup;
    }
    prev_val = temp->as.value;
    free_temp_node_parts(temp);
  }

  *result_node = get_bool_node(all_true);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return retval;
fail_cleanup:
  free_temp_node_parts(temp);
  return retval;
};

/**
 * @brief Returns the minimum or maximum value among the arguments as a NUMBER
 * node. All arguments must evaluate to NUMBER nodes or a syntax error is
 * returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result NUMBER node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_min_max(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval = ERR_NO_ERROR;
  int min_max_value;
  astnode *temp;

  for (int i = 1; i < list_node->as.list.count; i++) {
    err = eval_node(list_node->as.list.children[i], &temp, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(temp->type != NUMBER, fail_cleanup, ERR_SYNTAX_ERROR);
    if (i == 1)
      min_max_value = temp->as.value;

    if (!strcmp(list_node->as.list.children[0]->as.symbol, "MIN")) {
      if (temp->as.value < min_max_value && i != 1) {
        min_max_value = temp->as.value;
      }
    } else if (!strcmp(list_node->as.list.children[0]->as.symbol, "MAX")) {
      if (temp->as.value > min_max_value && i != 1) {
        min_max_value = temp->as.value;
      }
    } else {
      retval = ERR_INTERNAL;
      goto fail_cleanup;
    }

    free_temp_node_parts(temp);
  }

  *result_node = get_number_node(min_max_value);
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

  return retval;
fail_cleanup:
  free_temp_node_parts(temp);
  return retval;
};

/**
 * @brief Sets a variable to a value and returns the updated variable node.
 * If the first argument is a symbol node, checks if the variable exists,
 * and initializes it if does not.
 * The first argument must evaluate to a variable node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the updated variable node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_set(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval = ERR_NO_ERROR;
  astnode *var_node = NULL, *value_node = NULL, *value_node_copy = NULL;

  err = eval_node(list_node->as.list.children[1], &var_node, env);
  RETURN_ERR_IF(err, err);

  if (var_node->type == SYMBOL) {
    if (!exists_var(var_node->as.symbol, env)) {
      err = add_empty_var(var_node->as.symbol, env);
      CLEANUP_WITH_ERR_IF(err, cleanup, err);
    }
    CLEANUP_WITH_ERR_IF(var_node->origin == TEMPORARY, cleanup, ERR_INTERNAL);
    /* Symbol can not be temporary, meaning we can lose the reference knowing it
     * will be freed later */
    err = eval_node(var_node, &var_node, env);
    CLEANUP_WITH_ERR_IF(err, cleanup, err);
  }

  CLEANUP_WITH_ERR_IF(var_node->origin != VARIABLE, cleanup,
                      ERR_NOT_A_VARIABLE);

  /* obtain value to asign */
  err = eval_node(list_node->as.list.children[2], &value_node, env);
  CLEANUP_WITH_ERR_IF(err, cleanup, err);
  CLEANUP_WITH_ERR_IF(value_node->type == SYMBOL, cleanup, ERR_SYNTAX_ERROR);

  /* make node copy with VARIABLE origin */
  err = make_deep_copy(value_node, &value_node_copy, VARIABLE);
  CLEANUP_WITH_ERR_IF(err, cleanup, err);

  /* free the previous variable content */
  free_node_content(var_node);
  /* copy the entire structure */
  *var_node = *value_node_copy;
  /* return reference to the new variable */
  *result_node = var_node;

  free(value_node_copy);
cleanup:
  free_temp_node_parts(var_node);
  free_temp_node_parts(value_node);
  return retval;
}

/**
 * @brief Returns the quoted argument node without evaluation.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the quoted node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_quote(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  *result_node = list_node->as.list.children[1];
  return ERR_NO_ERROR;
}

/**
 * @brief Evaluates arguments and returns a new LIST node containing the
 * results.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result LIST node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_list(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t retval = ERR_NO_ERROR, err;
  astnode *new_list = get_list_node(), *temp = NULL;
  RETURN_ERR_IF(!new_list, ERR_OUT_OF_MEMORY);
  new_list->origin = TEMPORARY;

  for (int i = 1; i < list_node->as.list.count; i++) {
    err = eval_node(list_node->as.list.children[i], &temp, env);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
    err = add_child_node(new_list, temp);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
  }
  *result_node = new_list;

  return retval;
fail_cleanup:
  free_temp_node_parts(temp);
  free_temp_node_parts(new_list);
  return retval;
}

/**
 * @brief Checks if the argument is atomic (not a list) and returns a BOOLEAN
 * node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result BOOLEAN node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_atom(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  int is_atomic;
  err_t err;
  astnode *temp;

  err = eval_node(list_node->as.list.children[1], &temp, env);
  RETURN_ERR_IF(err, err);

  is_atomic = (temp->type != LIST);
  free_temp_node_parts(temp);

  *result_node = get_bool_node(is_atomic);
  (*result_node)->origin = TEMPORARY;
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  return ERR_NO_ERROR;
}

/**
 * @brief Returns the first element of a list argument.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the CAR node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_car(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err, retval = ERR_NO_ERROR;
  astnode *temp;

  err = eval_node(list_node->as.list.children[1], &temp, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(temp->type != LIST || temp->as.list.count < 1,
                      fail_cleanup, ERR_SYNTAX_ERROR);
  CLEANUP_WITH_ERR_IF(!temp->as.list.children[0], fail_cleanup, ERR_INTERNAL);

  *result_node = temp->as.list.children[0];

  /* free all other nodes in the list that are temporary, but not the one we
   * want to return */
  if ((*result_node)->origin == TEMPORARY) {
    (*result_node)->origin = UNSET;
    free_temp_node_parts(temp);
    (*result_node)->origin = TEMPORARY;
  } else {
    free_temp_node_parts(temp);
  };

  return retval;
fail_cleanup:
  free_temp_node_parts(temp);
  return retval;
}

/**
 * @brief Returns the rest of the list after the first element as a list node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the CDR list node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_cdr(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t retval = ERR_NO_ERROR, err;
  astnode *new_list = get_list_node(), *arg_node = NULL;
  RETURN_ERR_IF(!new_list, ERR_OUT_OF_MEMORY);
  new_list->origin = TEMPORARY;

  err = eval_node(list_node->as.list.children[1], &arg_node, env);
  CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
  CLEANUP_WITH_ERR_IF(arg_node->type != LIST || arg_node->as.list.count < 2,
                      fail_cleanup, ERR_SYNTAX_ERROR);
  for (int i = 1; i < arg_node->as.list.count; i++) {
    err = add_child_node(new_list, arg_node->as.list.children[i]);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
  }

  *result_node = new_list;

  /* the first item of the argument list would leak if its temporary */
  free_temp_node_parts(arg_node->as.list.children[0]);
  if (arg_node->origin == TEMPORARY) {
    free(arg_node->as.list.children);
    free(arg_node);
  }

  return retval;
fail_cleanup:
  free_temp_node_parts(arg_node);
  free_temp_node_parts(new_list);
  return retval;
}

/**
 * @brief Returns the nth element of a list argument without evaluating it.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the nth node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_nth(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  int nth;
  err_t err, retval = ERR_NO_ERROR;
  astnode *temp = NULL;

  err = eval_node(list_node->as.list.children[1], &temp, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(temp->type != NUMBER, fail_cleanup, ERR_SYNTAX_ERROR);

  nth = temp->as.value;
  free_temp_node_parts(temp);

  err = eval_node(list_node->as.list.children[2], &temp, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(temp->type != LIST || nth >= temp->as.list.count,
                      fail_cleanup, ERR_SYNTAX_ERROR);
  CLEANUP_WITH_ERR_IF(!temp->as.list.children[nth], fail_cleanup, ERR_INTERNAL);

  *result_node = temp->as.list.children[nth];

  /* free all other nodes in the list that are temporary, but not the one we
   * want to return */
  if ((*result_node)->origin == TEMPORARY) {
    (*result_node)->origin = UNSET;
    free_temp_node_parts(temp);
    (*result_node)->origin = TEMPORARY;
  } else {
    free_temp_node_parts(temp);
  }

  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(temp);
  return retval;
}

/**
 * @brief Returns the length of a list argument as a NUMBER node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result NUMBER node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_len(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env || !result_node,
                ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  int len;
  err_t err, retval = ERR_NO_ERROR;
  astnode *temp;

  err = eval_node(list_node->as.list.children[1], &temp, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(temp->type != LIST, cleanup, ERR_SYNTAX_ERROR);

  len = temp->as.list.count;
  free_temp_node_parts(temp);

  *result_node = get_number_node(len);
  CLEANUP_WITH_ERR_IF(!*result_node, cleanup, ERR_OUT_OF_MEMORY);
  (*result_node)->origin = TEMPORARY;

cleanup:
  free_temp_node_parts(temp);
  return retval;
}

/**
 * @brief Evaluates a conditional expression and returns the result of the
 * true(3rd arg) or false branch(4th arg).
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_if(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3 || list_node->as.list.count > 4,
                ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  int truthy;
  err_t err, retval = ERR_NO_ERROR;
  astnode *cond_node = NULL, *temp = NULL;

  err = eval_node(list_node->as.list.children[1], &cond_node, env);
  RETURN_ERR_IF(err, err);
  CLEANUP_WITH_ERR_IF(cond_node->type != BOOLEAN, fail_cleanup,
                      ERR_SYNTAX_ERROR);

  truthy = cond_node->as.value;
  free_temp_node_parts(cond_node);

  /* condition false and no negative branch */
  if (!truthy && list_node->as.list.count == 3) {
    *result_node = get_bool_node(truthy);
    (*result_node)->origin = TEMPORARY;
    RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
    return ERR_NO_ERROR;
  }

  err = eval_node(list_node->as.list.children[truthy ? 2 : 3], &temp, env);
  RETURN_ERR_IF(err, err);
  *result_node = temp;

  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(cond_node);
  return retval;
}

/**
 * @brief Evaluates a while loop, executing the body while the condition is
 * true.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_while(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  int while_cond, brk_stop = 0;
  err_t err, retval = ERR_NO_ERROR;
  astnode *cond_node = NULL, *temp = NULL;

  while (!brk_stop) {
    err = eval_node(list_node->as.list.children[1], &cond_node, env);
    RETURN_ERR_IF(err, err);
    CLEANUP_WITH_ERR_IF(cond_node->type != BOOLEAN, fail_cleanup,
                        ERR_SYNTAX_ERROR);

    while_cond = cond_node->as.value;
    free_temp_node_parts(cond_node);
    if (!while_cond)
      break;
    for (int i = 2; i < list_node->as.list.count; i++) {
      err = eval_node(list_node->as.list.children[i], &temp, env);
      if (err == CONTROL_BREAK) {
        brk_stop = 1;
        break;
      }
      RETURN_ERR_IF(err, err);
      free_temp_node_parts(temp);
    }
  }

  *result_node = get_bool_node(0);
  (*result_node)->origin = TEMPORARY;
  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);

  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(cond_node);
  return retval;
}

/**
 * @brief Breaks out of a loop, returning CONTROL_BREAK.
 * @param list_node List node containing the operator
 * @param result_node out param pointer, always NULL
 * @param env The environment for variable lookup and evaluation
 * @return err_t (CONTROL_BREAK)
 */
err_t oper_brk(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 1, ERR_SYNTAX_ERROR);
  *result_node = NULL;
  return CONTROL_BREAK;
}

/**
 * @brief Quits the REPL or program, returning CONTROL_QUIT.
 * @param list_node List node containing the operator
 * @param result_node out param pointer, always NULL
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_quit(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 1, ERR_SYNTAX_ERROR);
  *result_node = NULL;
  return CONTROL_QUIT;
}

/**
 * @brief Prints the evaluated argument via the print_node function and returns
 * it.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the printed node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_print(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err;
  astnode *node_to_print;

  err = eval_node(list_node->as.list.children[1], &node_to_print, env);
  RETURN_ERR_IF(err, err);

  print_node(node_to_print);
  printf("\n");

  *result_node = node_to_print;
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
    {"MAX", oper_min_max},
    {"MIN", oper_min_max},
    /* func */
    {"QUOTE", oper_quote},
    {"SET", oper_set},
    {"LIST", oper_list},
    {"ATOM", oper_atom},
    {"CAR", oper_car},
    {"CDR", oper_cdr},
    {"NTH", oper_nth},
    {"LENGTH", oper_len},
    /* control */
    {"IF", oper_if},
    {"WHILE", oper_while},
    {"BRK", oper_brk},
    {"PRINT", oper_print},
    {"QUIT", oper_quit},
};

int oper_count = sizeof(operators) / sizeof(operators[0]);