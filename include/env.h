#ifndef ENV_H
#define ENV_H

struct var_record{
  char* symbol;
  int value;
};

typedef struct Env {
  struct var_record* vars;
} env;

#endif