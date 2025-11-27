#include "repl.h"
#include "env.h"
#include "err.h"
#include "macros.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define INPUT_BUFF_SIZE 1024

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
err_t repl() {
  err_t err, retval = ERR_NO_ERROR;
  int curr_len = 0, braces = 0, accum_len = 0;
  char *buff, *accumulated = NULL, *line, *temp;

  /* Allocate buffer for user input */
  buff = malloc(INPUT_BUFF_SIZE);
  RETURN_ERR_IF(!buff, ERR_OUT_OF_MEMORY);

  /* Create a new environment for the REPL session */
  env *env = create_env();
  RETURN_ERR_IF(!env, ERR_OUT_OF_MEMORY);

  printf("> ");
  while ((line = fgets(buff, INPUT_BUFF_SIZE, stdin))) {
    /* Remove trailing newline from input */
    line[strcspn(line, "\n")] = '\0';
    curr_len = strlen(line);

    /* Count parentheses to track expression completeness */
    for (char *ptr = line; *ptr; ptr++) {
      switch (*ptr) {
      case '(':
        braces++;
        break;
      case ')':
        braces--;
        /* More closing than opening parens */
        CLEANUP_WITH_ERR_IF(braces < 0, cleanup, ERR_SYNTAX_ERROR);
        break;
      default:
        continue;
      }
    }

    /* Accumulate input lines until expression is complete */
    temp = realloc(accumulated, accum_len + curr_len + 1);
    CLEANUP_WITH_ERR_IF(!temp, cleanup, ERR_OUT_OF_MEMORY);
    accumulated = temp;
    memcpy(accumulated + accum_len, line, curr_len);
    accum_len += curr_len;
    accumulated[accum_len] = '\0';

    /* If parentheses are not balanced, prompt for more input */
    if (braces) {
      for (int i = 0; i < braces; i++) {
        printf("| ... ");
      }
      printf("> ");
      continue;
    }

    /* Process the complete code block */
    err = process_code_block(accumulated, 1, env);
    if (err == CONTROL_QUIT) 
      break;
    if (err == CONTROL_BREAK) 
      err = ERR_SYNTAX_ERROR;
    CLEANUP_WITH_ERR_IF(err, cleanup, err);
    /* Reset accumulator for next expression */
    accum_len = 0; 
  }

  /* If input ended with unbalanced parentheses, return syntax error */
  if (braces)
    retval = ERR_SYNTAX_ERROR;

cleanup:
  free(buff);
  free(accumulated);
  free_env(env);
  return retval;
}
