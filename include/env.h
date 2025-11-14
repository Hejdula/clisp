#ifndef ENV_H
#define ENV_H

typedef struct ASTnode astnode;

struct var_record {
  char *symbol;
  astnode *node;
};

typedef struct Env {
  struct var_record *vars;
  int var_count;
} env;

astnode *get_var(char *var_name, env *env);

#endif