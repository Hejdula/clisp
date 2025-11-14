#include "env.h"
#include "macros.h"
#include <stdlib.h>
#include <string.h>
#include "ast.h"

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

err_t set_var(const char *var_name, astnode *node, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!var_name || !*var_name || !env, ERR_INTERNAL);

  /* update existing */
  for (int i = 0; i < env->var_count; ++i) {
    if (!strcmp(var_name, env->vars[i].symbol)) {
      env->vars[i].node = node;
      return ERR_NO_ERROR;
    }
  }

  /* add new */
  struct var_record *tmp =
      realloc(env->vars, (env->var_count + 1) * sizeof(*tmp));
  RETURN_ERR_IF(!tmp, ERR_OUT_OF_MEMORY);
  env->vars = tmp;

  size_t len = strlen(var_name);
  char *sym = malloc(len + 1);
  RETURN_ERR_IF(!sym, ERR_OUT_OF_MEMORY);
  memcpy(sym, var_name, len + 1);

  env->vars[env->var_count].symbol = sym;
  env->vars[env->var_count].node = node;
  env->var_count++;

  return ERR_NO_ERROR;
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
  return e;
}

void free_env(env *env) {
  if (env) {
    for(int i = 0; i < env->var_count; i++){
      free_node(env->vars->node);
      free(env->vars->symbol);
    }
    free(env->vars);
  }
  free(env);
}