#include "env.h"
#include "macros.h"
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