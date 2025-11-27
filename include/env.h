#ifndef ENV_H
#define ENV_H

#include "err.h"

typedef struct ASTnode astnode;

/**
 * @brief A pair of name and variable content
 */
struct var_record {
  char *symbol;
  astnode *node;
};

/**
 * @brief Environment for evaluation of the AST tree, provides variable storage
 * and lookup
 */
typedef struct Env {
  struct var_record *vars;
  int var_count;
} env;

/**
 * @brief Create a env object and allocate minimum necessary memory
 *
 * @return env*
 */
env *create_env(void);

/**
 * @brief Frees all variables and allocated memory in the environment
 *
 * @param env to free
 */
void free_env(env *e);

/**
 * @brief Gets the variable of the given name from the environment.
 * Returns NULL if the variable does not exist in the environment
 * @param var_name Name of the variable
 * @param env Environment to search the variable in
 * @return astnode* | NULL
 */
astnode *get_var(const char *var_name, env *env);

/**
 * @brief Creates a new variable in the environment.
 * Initializes the variable to number with value 0
 *
 * @param var_name Name of the variable
 * @param env Environment to operate in
 * @return err_t
 */
err_t add_empty_var(const char *var_name, env *env);

/**
 * @brief Check if variable exists in the environment
 *
 * @param var_name Name of the variable
 * @param env Environment to search in
 * @return int 1 if exists, 0 otherwise
 */
int exists_var(const char *var_name, const env *env);

/**
 * @brief Prints all variables in the environment
 *
 * @param e environment to vizualize
 */
void print_env(const env *env);

#endif