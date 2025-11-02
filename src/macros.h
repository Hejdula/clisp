#ifndef MACROS
#define MACROS

#include "err.h"
#include <errno.h>
#include <stdio.h>

/*
 * Log the error and the system errno when VERBOSE is set.
 */
#define LOG_IF_VERBOSE(err)                                                    \
  do {                                                                         \
    if (DBG_VERBOSE) {                                                         \
      fprintf(stderr, "Error: %s at %s:%d\n", err_str(err), __FILE__,          \
              __LINE__);                                                       \
      if (errno)                                                               \
        perror("system");                                                      \
    }                                                                          \
  } while (0)

/*
 * Set the `retval` to `err` and jump to `label` for cleanup.
 */
#define CLEANUP_WITH_ERR_IF(cond, label, err)                                  \
  do {                                                                         \
    if (cond) {                                                                \
      LOG_IF_VERBOSE(err);                                                     \
      retval = (err);                                                          \
      goto label;                                                              \
    }                                                                          \
  } while (0)

/**
 * @brief asdfadsf
 *
 */
#define RETURN_ERR_IF(cond, err)                                               \
  do {                                                                         \
    if (cond) {                                                                \
      LOG_IF_VERBOSE(err);                                                     \
      return (err);                                                            \
    }                                                                          \
  } while (0)

#endif
