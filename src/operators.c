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
    free_temp_node_parts(temp_node);
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
    free_temp_node_parts(temp_node);
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
    free_temp_node_parts(temp_node);
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
  int i, res = 0;

  astnode *temp_node = NULL;
  for (i = 1; i < list_node->as.list.count; i++) {
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

    err = eval_node(list_node->as.list.children[i], &temp_node, env);
    RETURN_ERR_IF(err, err);
    RETURN_ERR_IF(temp_node->type != NUMBER, ERR_SYNTAX_ERROR);
    res = (i == 1 ? temp_node->as.value : res / temp_node->as.value);
    free_temp_node_parts(temp_node);
  }

  *result_node = get_number_node(res);
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
  free_temp_node_parts(var_node);
  free_temp_node_parts(value_node);
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
  free_temp_node_parts(var_node);
  free_temp_node_parts(value_node);
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

err_t oper_min_max(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
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
  err = make_deep_copy(value_node, &value_node_copy, VARIABLE);
  CLEANUP_WITH_ERR_IF(err, cleanup, err);

  free_node_content(var_node);
  *var_node = *value_node_copy;
  *result_node = var_node;

  free(value_node_copy);
cleanup:
  free_temp_node_parts(var_node);
  free_temp_node_parts(value_node);
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

err_t oper_list(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
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

err_t oper_atom(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
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

err_t oper_car(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count != 2, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  err_t err;
  astnode *temp;

  err = eval_node(list_node->as.list.children[1], &temp, env);
  RETURN_ERR_IF(err, err);
  RETURN_ERR_IF(temp->type != LIST || temp->as.list.count < 1,
                ERR_SYNTAX_ERROR);
  RETURN_ERR_IF(!temp->as.list.children[0], ERR_INTERNAL);

  *result_node = temp->as.list.children[0];

  free_temp_node_parts(temp);

  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  return ERR_NO_ERROR;
}

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

err_t oper_nth(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
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
  free_temp_node_parts(temp);

  RETURN_ERR_IF(!*result_node, ERR_OUT_OF_MEMORY);
  return ERR_NO_ERROR;
fail_cleanup:
  free_temp_node_parts(temp);
  return retval;
}

err_t oper_len(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
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

err_t oper_while(astnode *list_node, astnode **result_node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!list_node || list_node->type != LIST || !env, ERR_INTERNAL);
  RETURN_ERR_IF(list_node->as.list.count < 3, ERR_SYNTAX_ERROR);
  for (int i = 0; i < list_node->as.list.count; i++)
    RETURN_ERR_IF(!list_node->as.list.children[i], ERR_INTERNAL);

  int while_cond;
  err_t err, retval = ERR_NO_ERROR;
  astnode *cond_node = NULL, *temp = NULL;

  while (1) {
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
      if (err == CONTROL_BREAK)
        break;
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
    {"PRINT", oper_print},
};

int oper_count = sizeof(operators) / sizeof(operators[0]);