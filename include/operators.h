#ifndef OPERATORS_H
#define OPERATORS_H

#include "err.h"
#include "ast.h"
#include "env.h"


/**
 * @brief Evaluates and sums the arguments and returns a new NUMBER node.
 * All arguments must evaluate to NUMBER nodes or a syntax error is returned.
 * @param list_node List node containing the operator and arguments
 * @param result_node out param pointer to the result NUMBER node with TEMPORARY
 * origin, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_add(astnode *list_node, astnode **result_node, env *env);

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
err_t oper_sub(astnode *list_node, astnode **result_node, env *env);

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
err_t oper_mul(astnode *list_node, astnode **result_node, env *env);

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
err_t oper_div(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Increments a variable by a value and returns the updated variable
 * node. The first argument must be a variable node, the second a NUMBER node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the updated variable node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_inc(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Decrements a variable by a value and returns the updated variable
 * node. The first argument must be a variable node, the second a NUMBER node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the updated variable node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_dec(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Checks if all arguments are equal and returns a BOOLEAN node.
 * All arguments must evaluate to NUMBER nodes or a syntax error is returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result BOOLEAN node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_eql(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Checks if all arguments are non-equal and returns a BOOLEAN node.
 * All arguments must evaluate to NUMBER nodes or a syntax error is returned.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result BOOLEAN node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_noneql(astnode *list_node, astnode **result_node, env *env);

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
err_t oper_grt_lwr(astnode *list_node, astnode **result_node, env *env);

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
err_t oper_min_max(astnode *list_node, astnode **result_node, env *env);

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
err_t oper_set(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Returns the quoted argument node without evaluation.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the quoted node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_quote(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Evaluates arguments and returns a new LIST node containing the
 * results.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result LIST node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_list(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Checks if the argument is atomic (not a list) and returns a BOOLEAN
 * node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result BOOLEAN node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_atom(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Returns the first element of a list argument.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the CAR node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_car(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Returns the rest of the list after the first element in a list node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the CDR list node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_cdr(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Returns the nth element of a list argument without evaluating it.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the nth node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_nth(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Returns the length of a list argument as a NUMBER node.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result NUMBER node, NULL on
 * failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_len(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Evaluates a conditional expression and returns the result of the
 * true(3rd arg) or false branch(4th arg).
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_if(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Evaluates a while loop, executing the body while the condition is
 * true.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the result node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_while(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Breaks out of a loop, returning CONTROL_BREAK.
 * @param list_node List node containing the operator
 * @param result_node out param pointer, always NULL
 * @param env The environment for variable lookup and evaluation
 * @return err_t (CONTROL_BREAK)
 */
err_t oper_brk(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Quits the REPL or program, returning CONTROL_QUIT.
 * @param list_node List node containing the operator
 * @param result_node out param pointer, always NULL
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_quit(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Prints the evaluated argument via the print_node function and returns
 * it.
 * @param list_node List node containing the operator
 * @param result_node out param pointer to the printed node, NULL on failure
 * @param env The environment for variable lookup and evaluation
 * @return err_t
 */
err_t oper_print(astnode *list_node, astnode **result_node, env *env);

/**
 * @brief Entry for an operator: symbol and function pointer.
 */
struct operator_entry {
  char *symbol; /**< Operator symbol as string. */
  err_t (*func)(astnode *list_node, astnode **result_node, env *env); /**< Function pointer for operator implementation. */
};

/**
 * @brief Array of all supported operators and their function pointers.
 */
extern struct operator_entry operators[];

/**
 * @brief Number of supported operators in the operators array.
 */
extern int oper_count;
#endif