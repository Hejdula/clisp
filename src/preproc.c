#include "err.h"
#include "macros.h"
#include <ctype.h>
#include <string.h>

/**
 * @brief Replaces Lisp comments and line separators with spaces
 *
 * @param code_string Lisp source code
 * @return -1 if code_string is NULL
 */
err_t preprocess(char *code_string) {
  /* sanity check */
  RETURN_ERR_IF(!code_string, ERR_INTERNAL);

  int i, commenting = 0;
  char c;
  for (i = 0;(c = code_string[i]);i++) {
    if (c == ';')
      commenting = 1;
    if (c == '\n' || c == '\r')
      commenting = 0;
    if (commenting || c == '\n' || c == '\r')
      code_string[i] = ' ';
  }
  return ERR_NO_ERROR;
};

/**
 * @brief Changes all aplha characters to uppercase
 * 
 * @param code_string 
 */
void to_upper_str(char *code_string) {
  if(!code_string) return;
  int i;
  char c;
  for (i = 0;(c = code_string[i]); i++) {
    code_string[i] = toupper(c);
  }
}