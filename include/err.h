#ifndef ERR_H
#define ERR_H

/** 
 * ERROR, status codes and control signals for parsing and evaluation of Lisp code
 */
typedef enum {
  ERR_NO_ERROR = 0,          // successful completion, no error
  ERR_INVALID_INPUT_FILE,    // invalid or missing input file name/format
  ERR_SYNTAX_ERROR,          // error in Lisp source code
  ERR_FILE_ACCESS_FAILURE,   // cannot read/write file
  ERR_OUT_OF_MEMORY,         // malloc/calloc/realloc failed
  ERR_INVALID_ARGS,          // invalid program CLI arguments
  ERR_INTERNAL,              // unexpected internal error (should not happen)
  ERR_RUNTIME_UNKNOWN_VAR,   // access to undefined variable
  ERR_NOT_A_VARIABLE,        // assignment target is not a variable node
  ERR_ZERO_DIVISON,          // division by zero in arithmetic operator
  ERR_UNKNOWN_OPERATOR,      // unsupported or unknown Lisp operator
  CONTROL_BREAK,             // control signal for (brk) inside loops
  CONTROL_QUIT,              // control signal for (quit) to exit program
} err_t;

/* Returns a static string for the error code. */
const char *err_msg(int code);

#endif