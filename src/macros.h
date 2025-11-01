#ifndef MACROS
#define MACROS

/* 
 * Set the `retval` to `err` and jump to `label` for cleanup.
 */
#define ERRCLEANUP(label, err) \
    do {                         \
        retval = (err);          \
        goto label;              \
    } while (0)

#endif