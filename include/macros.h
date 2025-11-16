#ifndef MACROS
#define MACROS

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
    if (err != CONTROL_BREAK) {                                                \
      fprintf(stderr, "Error: %s at %s:%d\n", err_msg(err), __FILE__,          \
              __LINE__);                                                       \
      if (errno)                                                               \
        perror("system");                                                      \
    }                                                                          \
  } while (0)
#else
/* No-op when verbose logging disabled at compile time */
#define LOG_IF_VERBOSE(err)                                                    \
  do {                                                                         \
    (void)0;                                                                   \
  } while (0)
#endif

/*
 * IF cond passes, sets the `retval` to `err` and goto `label`.
 */
#define CLEANUP_WITH_ERR_IF(cond, label, err)                                  \
  do {                                                                         \
    if (cond) {                                                                \
      LOG_IF_VERBOSE(err);                                                     \
      retval = (err);                                                          \
      goto label;                                                              \
    }                                                                          \
  } while (0)

#define CLEANUP_IF(cond, label)                                                \
  do {                                                                         \
    if (cond) {                                                                \
      goto label;                                                              \
    }                                                                          \
  } while (0)

/**
 * @brief
 *
 */
#define RETURN_ERR_IF(cond, err)                                               \
  do {                                                                         \
    if (cond) {                                                                \
      LOG_IF_VERBOSE(err);                                                     \
      return (err);                                                            \
    }                                                                          \
  } while (0)

/**
 *
 */
#define RETURN_VAL_IF(cond, retval)                                            \
  do {                                                                         \
    if (cond) {                                                                \
      return (retval);                                                         \
    }                                                                          \
  } while (0)

#define RETURN_NULL_IF(cond)                                                   \
  do {                                                                         \
    if (cond) {                                                                \
      return ((void *)0);                                                      \
    }                                                                          \
  } while (0)

#endif