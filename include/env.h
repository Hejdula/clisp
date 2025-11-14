#ifndef ENV_H
#define ENV_H

#include "err.h"

typedef struct ASTnode astnode;

struct var_record {
  char *symbol;
  astnode *node;
};

typedef struct Env {
  struct var_record *vars;
  int var_count;
} env;

env *create_env(void);
void free_env(env *e);

astnode *get_var(char *var_name, env *env);
err_t set_var(const char *var_name, astnode *node, env *env);

int has_var(const char *var_name, const env *env);

#endif