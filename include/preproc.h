#ifndef PREPROC_H
#define PREPROC_H

#include "err.h"

/**
 * @brief Removes lisp comments from the code_string, converts all alphas to
 * uppercase and removes line separators
 *
 * @param code_string Lisp source code
 * @return -1 if code_string is NULL
 */
err_t preprocess(char *code_string);

#endif