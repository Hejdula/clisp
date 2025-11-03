#include "lexer.h"
#include "err.h"
#include "macros.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Fills token_indices and token_lenghts by tokens from source_code,
 * token_founds by " " and produces a separate token for each bracket and "'"
 * up to caller to free token_indices and token_lenghts
 *
 * @param source_code Lisp code to tokenize
 * @param token_indices allocates and fill with indices into source code for
 * each token
 * @param token_lenghts allocates and fills with lengths of tokens
 * @return number of tokens or negative errcodes
 */
int tokenize(char *source_code, int **token_indices, int **token_lenghts) {
  int source_length, i, token_count = 0, curr_len = 0, token_found = 0;
  char c;

  source_length = strlen(source_code);
  *token_indices = calloc(source_length * sizeof(int *), 1);
  RETURN_ERR_IF(!*token_indices, -ERR_OUT_OF_MEMORY);
  *token_lenghts = calloc(source_length * sizeof(int *), 1);
  if (!*token_lenghts) {
    free(token_indices);
    return -ERR_OUT_OF_MEMORY;
  }

  for (i = 0; i < source_length; i++) {
    c = source_code[i];

    /* skip spacces, end previous tokens */
    if (c == ' ') {
      if (token_found) {
        (*token_lenghts)[token_count - 1] = curr_len;
        curr_len = 0;
        token_found = 0;
      }
      continue;
    }

    /* make single token out of quotes and brackets, end previous tokens */
    if (c == '\'' || c == '(' || c == ')') {
      if (token_found) {
        token_found = 0;
        (*token_lenghts)[token_count - 1] = curr_len;
        curr_len = 0;
      }
      (*token_indices)[token_count] = i;
      (*token_lenghts)[token_count] = 1;
      token_count++;
      continue;
    }

    /* found a character, if its first token found, then add to indices */
    if (!token_found) {
      (*token_indices)[token_count] = i;
      token_count++;
      token_found = 1;
    }
    curr_len++;
  }

  if (token_found) {
    (*token_lenghts)[token_count - 1] = curr_len;
  }

  return token_count;
};