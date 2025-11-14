#include "lexer.h"
#include "err.h"
#include "macros.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Helper function to realloc *tokens array and append a copy of length
 * token_len from source, on success increases token_count
 *
 * @param tokens array of already produced tokens
 * @param token_count current count of tokens
 * @param token_len how many bytes from source to copy
 * @param source from where to copy
 * @return err_t
 */
err_t add_token(char ***tokens, int *token_count, int token_len,
                const char *source) {
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
 * @brief Tokenize Lisp-like source into strings.
 *
 * Splits on spaces (' ') and tabs and treats each of the characters `'`, `(`, `)`
 * as standalone one-character tokens. The input is not modified.
 *
 * On success, allocates a NULL-terminated array of N heap-allocated,
 * NULL-terminated token strings and stores it in *tokens.
 *
 * Its caller responsibility to free each token and the token array on success
 *
 * @param source_code NUL-terminated input string.
 * @param tokens out param; must point to a char** initialized to NULL.
 * @return number of tokens on success; negative err_t on failure.
 */
int tokenize(const char *source_code, char ***tokens) {
  int source_length, i, retval = ERR_NO_ERROR, token_count = 0, curr_len = 0,
                        token_found = 0, err;
  char c, **tmp;

  /* sanity check */
  RETURN_ERR_IF(!source_code || !tokens || *tokens, -ERR_INTERNAL);

  source_length = strlen(source_code);
  for (i = 0; i < source_length; i++) {
    c = source_code[i];

    /* skip spacces, add previous tokens */
    if (isblank(c)) {
      if (token_found) {
        err = add_token(tokens, &token_count, curr_len,
                        source_code + i - curr_len);
        CLEANUP_WITH_ERR_IF(err, fail_cleanup, -err);

        curr_len = 0;
        token_found = 0;
      }
      continue;
    }

    /* make single token out of quotes and brackets, add previous tokens */
    if (c == '\'' || c == '(' || c == ')') {
      if (token_found) {
        err = add_token(tokens, &token_count, curr_len,
                        source_code + i - curr_len);
        CLEANUP_WITH_ERR_IF(err, fail_cleanup, -err);

        token_found = 0;
        curr_len = 0;
      }

      err = add_token(tokens, &token_count, 1, source_code + i - curr_len);
      CLEANUP_WITH_ERR_IF(err, fail_cleanup, -err);
      continue;
    }

    /* found a token, start counting curr_len from beginning*/
    if (!token_found) {
      token_found = 1;
    }
    curr_len++;
  }

  /* add the last token */
  if (token_found) {
    err = add_token(tokens, &token_count, curr_len,
                    source_code + source_length - curr_len);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, -err);
  }

  /* NULL-terminate the token_array */
  tmp = realloc(*tokens, (token_count + 1) * sizeof(char *));
  CLEANUP_WITH_ERR_IF(!tmp, fail_cleanup, -ERR_OUT_OF_MEMORY);
  *tokens = tmp;
  (*tokens)[token_count] = NULL;

  return token_count;

fail_cleanup: /* free already allocated tokens on failure */
  for (i = 0; i < token_count; i++) {
    free((*tokens)[i]);
  }
  free(*tokens);
  *tokens = NULL;
  return retval;
};
