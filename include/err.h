#ifndef ERR_H
#define ERR_H

/* Simple project error codes header
 * - Provides `err_t` enum values
 * - Header-only helper `err_str()` as `static inline` so no separate .c is
 * needed
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
  CONTROL_BREAK,
  CONTROL_QUIT,
} err_t;

/* Returns a static string for the error code. */
static inline const char *err_msg(int code) {
  if(code < 0) code = -code;
  switch (code) {
  case ERR_NO_ERROR:
    return "OK";
  case ERR_INVALID_INPUT_FILE:
    return "Invalid input file";
  case ERR_SYNTAX_ERROR:
    return "Syntax error";
  case ERR_FILE_ACCESS_FAILURE:
    return "File access failure";
  case ERR_OUT_OF_MEMORY:
    return "Out of memory";
  case ERR_INVALID_ARGS:
    return "Invalid arguments of the program";
  case ERR_INTERNAL:
    return "something went really wrong";
  case ERR_RUNTIME_UNKNOWN_VAR:
    return "Unknown variable";
  case ERR_NOT_A_VARIABLE:
    return "Can not assign, target is not a variable";
  default:
    return "Unknown error";
  }
}

#endif