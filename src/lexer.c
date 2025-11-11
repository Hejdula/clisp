#include "lexer.h"
#include "err.h"
#include "macros.h"
#include <stdlib.h>
#include <string.h>

err_t add_token(char ***tokens, int *token_count, int token_len, char *source) {
  char **tmp;
  tmp = realloc(*tokens, (*token_count + 1) * sizeof(char **));
  RETURN_ERR_IF(!tmp, ERR_OUT_OF_MEMORY);
  *tokens = tmp;

  (*tokens)[*token_count] = calloc(1, (token_len + 1) * sizeof(char));
  RETURN_ERR_IF(!(*tokens)[*token_count], ERR_OUT_OF_MEMORY);
  memcpy((*tokens)[*token_count], source, token_len);

  (*token_count)++;

  return ERR_NO_ERROR;
}

/**
 * @brief Fills token_indices and token_lenghts by tokens from source_code,
 * token_founds by " " and produces a separate token for each bracket and "'"
 * up to caller to free token_indices and token_lenghts
 *
 * @param source_code Lisp code to tokenize
 * @param token_indices allocates and fill with indices into source code for
 * each token
 * @param token_lenghts allocates and fills with lengths of tokens
 * @return number of tokens or negative err_t
 */
int tokenize(char *source_code, char ***tokens) {
  int source_length, i, token_count = 0, curr_len = 0, token_found = 0, err;
  char c;

  /* sanity check */
  RETURN_ERR_IF(!source_code || !tokens, -ERR_INTERNAL);

  source_length = strlen(source_code);
  for (i = 0; i < source_length; i++) {
    c = source_code[i];

    /* skip spacces, end previous tokens */
    if (c == ' ') {
      if (token_found) {

        err = add_token(tokens, &token_count, curr_len,
                        source_code + i - curr_len);
        RETURN_ERR_IF(err, -err);

        curr_len = 0;
        token_found = 0;
      }
      continue;
    }

    /* make single token out of quotes and brackets, end previous tokens */
    if (c == '\'' || c == '(' || c == ')') {
      if (token_found) {
        err = add_token(tokens, &token_count, curr_len,
                        source_code + i - curr_len);
        RETURN_ERR_IF(err, -err);

        token_found = 0;
        curr_len = 0;
      }

      err = add_token(tokens, &token_count, 1, source_code + i - curr_len);
      RETURN_ERR_IF(err, -err);
      continue;
    }

    /* found a character, if its first token found, add it to indices */
    if (!token_found) {
      token_found = 1;
    }
    curr_len++;
  }

  if (token_found) {
    err = add_token(tokens, &token_count, curr_len,
                    source_code + source_length - curr_len);
    RETURN_ERR_IF(err, -err);
  }

  return token_count;
};
