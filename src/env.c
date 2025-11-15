#include "env.h"
#include "ast.h"
#include "err.h"
#include "macros.h"
#include <stdlib.h>
#include <string.h>

astnode *get_var(char *var_name, env *env) {
  /* sanity check */
  RETURN_NULL_IF(!var_name || !env);

  int i;
  for (i = 0; i < env->var_count; i++) {
    if (!strcmp(var_name, env->vars[i].symbol)) {
      return env->vars[i].node;
    }
  }
  return NULL;
}

err_t add_empty_var(const char *var_name, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!var_name || !*var_name || !env, ERR_INTERNAL);

  err_t retval = ERR_NO_ERROR;
  astnode *dummy_node = get_number_node(0);
  RETURN_ERR_IF(!dummy_node, ERR_OUT_OF_MEMORY);
  dummy_node->origin = VARIABLE;

  /* add new */
  struct var_record *tmp =
      realloc(env->vars, (env->var_count + 1) * sizeof(*tmp));
  CLEANUP_WITH_ERR_IF(!tmp, fail_cleanup, ERR_OUT_OF_MEMORY);
  env->vars = tmp;

  size_t len = strlen(var_name);
  char *sym = malloc(len + 1);
  CLEANUP_WITH_ERR_IF(!sym, fail_cleanup, ERR_OUT_OF_MEMORY);
  memcpy(sym, var_name, len + 1);

  env->vars[env->var_count].symbol = sym;
  env->vars[env->var_count].node = dummy_node;
  env->var_count++;

  return ERR_NO_ERROR;
fail_cleanup:
  free_node(dummy_node);
  return retval;
}

int exists_var(const char *var_name, const env *env) {
  RETURN_VAL_IF(!var_name || !env, 0);
  for (int i = 0; i < env->var_count; ++i) {
    if (!strcmp(var_name, env->vars[i].symbol))
      return 1;
  }
  return 0;
}

env *create_env() {
  env *e = malloc(sizeof(struct Env));
  RETURN_NULL_IF(!e);
  e->var_count = 0;
  e->vars = malloc(sizeof(struct var_record));
  if (!e->vars) {
    free(e);
    return NULL;
  };
  return e;
}

void free_env(env *env) {
  if (env) {
    for (int i = 0; i < env->var_count; i++) {
      free_node(env->vars[i].node);
      free(env->vars[i].symbol);
    }
    free(env->vars);
  }
  free(env);
}

void print_env(const env *e) {
  if (!e) {
    fputs("<env NULL>\n", stdout);
    return;
  }
  if (e->var_count == 0) {
    fputs("<env empty>\n", stdout);
    return;
  }
  for (int i = 0; i < e->var_count; ++i) {
    if (i == 0) fputs("Environment variables:\n", stdout);
    fputs("  ", stdout);
    fputs(e->vars[i].symbol ? e->vars[i].symbol : "<nil>", stdout);
    fputs(" => ", stdout);
    if (e->vars[i].node) {
      print_node(e->vars[i].node);
    } else {
      fputs("NIL", stdout);
    }
    fputc('\n', stdout);
  }
}

