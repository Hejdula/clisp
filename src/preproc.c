#include "err.h"
#include "macros.h"
#include <ctype.h>
#include <string.h>

/**
 * @brief Removes comments from the code_string, converts all alphas to
 * uppercase and removes line separators
 *
 * @param code_string Lisp source code
 * @return -1 if code_string is NULL
 */
err_t preprocess(char *code_string) {
  /* sanity check */
  RETURN_ERR_IF(!code_string, ERR_INTERNAL);

  int i = 0, commenting = 0;
  char c;
  while ((c = code_string[i])) {
    if (c == ';')
      commenting = 1;
    if (c == '\n' || c == '\r')
      commenting = 0;
    if (commenting || c == '\n' || c == '\r')
      code_string[i] = ' ';
    else
      code_string[i] = toupper(c);
    i++;
  }
  return ERR_NO_ERROR;
};