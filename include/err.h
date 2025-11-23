#ifndef ERR_H
#define ERR_H

/** 
 * ERROR, status codes and control signals for parsing and evaluation of Lisp code
 */
typedef enum {
  ERR_NO_ERROR = 0,
  ERR_INVALID_INPUT_FILE,
  ERR_SYNTAX_ERROR,
  ERR_FILE_ACCESS_FAILURE,
  ERR_OUT_OF_MEMORY,
  ERR_INVALID_ARGS,
  ERR_INTERNAL,
  ERR_RUNTIME_UNKNOWN_VAR,
  ERR_NOT_A_VARIABLE,
  ERR_ZERO_DIVISON,
  CONTROL_BREAK,
  CONTROL_QUIT,
} err_t;

/* Returns a static string for the error code. */
const char *err_msg(int code);

#endif