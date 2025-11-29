#include "err.h"
#include <errno.h>
#include <stdio.h>

int first_call = 1;

/**
 * @brief Logs error with given file and line
 * Prints the error msg when first time called,
 * then appends path trace to the first error cal
 *
 * @param err
 * @param file file in which the error occured
 * @param line line at which the erorr occured
 */
void log_err_if_verb(err_t err, const char *file, int line) {
  extern int first_call;
  if (first_call) {
    fprintf(stderr, "Error: %s detected at %s:%d\n", err_msg(err), file, line);
    if (errno)
      perror("system");
    first_call = 0;
  } else {
    fprintf(stderr, " ==> %s:%d\n", file, line);
  }
}
