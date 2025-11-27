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

  buff = malloc(INPUT_BUFF_SIZE);
  RETURN_ERR_IF(!buff, ERR_OUT_OF_MEMORY);

  env *env = create_env();
  RETURN_ERR_IF(!env, ERR_OUT_OF_MEMORY);

  printf(">");
  while ((line = fgets(buff, INPUT_BUFF_SIZE, stdin))) {
    line[strcspn(line, "\n")] = '\0';
    curr_len = strlen(line);

    for (char *ptr = line; *ptr; ptr++) {
      switch (*ptr) {
      case '(':
        braces++;
        break;
      case ')':
        braces--;
        CLEANUP_WITH_ERR_IF(braces < 0, cleanup, ERR_SYNTAX_ERROR);
        break;
      default:
        continue;
      }
    }

    temp = realloc(accumulated, accum_len + curr_len + 1);
    CLEANUP_WITH_ERR_IF(!temp, cleanup, ERR_OUT_OF_MEMORY);
    accumulated = temp;
    memcpy(accumulated + accum_len, line, curr_len);
    accum_len += curr_len;
    accumulated[accum_len] = '\0';

    if (braces) {
      for (int i = 0; i < braces; i++) {
        printf("| ... ");
      }
      printf(">");
      continue;
    }

    err = process_code_block(accumulated, 1, env);
    if (err == CONTROL_QUIT) {
      printf(">");
      break;
    }
    CLEANUP_WITH_ERR_IF(err, cleanup, err);
    accum_len = 0;
  }

  if (braces)
    retval = ERR_SYNTAX_ERROR;

cleanup:
  printf("%s>", line);
  free(buff);
  free(accumulated);
  free_env(env);
  return retval;
}
