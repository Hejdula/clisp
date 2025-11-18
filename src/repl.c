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

int repl() {
  err_t err, retval = ERR_NO_ERROR;
  int curr_len = 0, braces = 0, accum_len = 0;
  char *buff, *accumulated = NULL, *line, *temp;

  buff = malloc(INPUT_BUFF_SIZE);
  RETURN_ERR_IF(!buff, ERR_OUT_OF_MEMORY);

  env *env = create_env();
  RETURN_ERR_IF(!env, ERR_OUT_OF_MEMORY);
	
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
      continue;
    }

    err = process_code_block(accumulated, 1, env);
    if (err == CONTROL_QUIT)
      break;
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
