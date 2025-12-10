#ifndef PREPROC_H
#define PREPROC_H

#include "err.h"

/**
 * @brief Replaces Lisp comments and line separators with spaces
 *
 * @param code_string Lisp source code
 * @return -1 if code_string is NULL
 */
err_t preprocess(char *code_string);

/**
 * @brief Changes all aplha characters to uppercase
 * 
 * @param code_string 
 * @return err_t 
 */
err_t to_upper_str(char *code_string);
#endif