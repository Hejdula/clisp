#include "err.h"
#include <ctype.h>
#include <string.h>

/**
 * @brief Removes comments from the code_string and makes it all uppercase
 *
 * @param code_string Lisp source code
 * @return -1 if code_string is NULL
 */
int preprocess(char *code_string) {
  if (code_string == NULL)
    return -1;
  int i = 0, commenting = 0;
  char c;
  while ((c = code_string[i])) {
    if (c == ';')
      commenting = 1;
    if (c == '\n')
      commenting = 0;
    if (commenting || c == '\n')
      code_string[i] = ' ';
    else 
      code_string[i] = toupper(c);
    i++;
  }
  return 0;
};