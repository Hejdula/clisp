#ifndef MACROS
#define MACROS

#include "err.h"
#include <errno.h>
#include <stdio.h>

/**
 * Compile-time debug logging switch.
 * Define DBG_VERBOSE to 1 to log error, file, line number and errno if set,
 * or 0 to omit all logging.
 */
#define DBG_VERBOSE 1

#if defined(DBG_VERBOSE) && DBG_VERBOSE
#define LOG_IF_VERBOSE(err)                                                    \
  do {                                                                         \
    fprintf(stderr, "Error: %s at %s:%d\n", err_msg(err), __FILE__, __LINE__); \
    if (errno)                                                                 \
      perror("system");                                                        \
  } while (0)
#else
/* No-op when verbose logging disabled at compile time */
#define LOG_IF_VERBOSE(err)                                                    \
  do {                                                                         \
    (void)0;                                                                   \
  } while (0)
#endif

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
