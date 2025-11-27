#include "err.h"

#ifndef REPL_H
#define REPL_H


/**
 * @brief Read-Eval-Print Loop (REPL) for the Lisp interpreter.
 *
 * Continuously reads lines of input from stdin, accumulates them until all
 * parentheses are balanced, then processes the complete code block. Handles
 * multi-line expressions, tracks parentheses balance, and prints a prompt for
 * incomplete expressions. Evaluates each complete expression and prints the
 * result.
 *
 * The REPL can be exited by sending EOF (Ctrl+D) or by evaluating an expression
 * that triggers CONTROL_QUIT.
 *
 * @return err_t Returns ERR_NO_ERROR on successful execution, or an error code
 * defined in err.h on failure.
 */
err_t repl();

#endif