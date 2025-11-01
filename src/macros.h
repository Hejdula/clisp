#ifndef MACROS
#define MACROS

/* 
 * Set the `retval` to `err` and jump to `label` for cleanup.
 */
#define CLEANUP_IF_FAIL(cond, label, err) \
    do {                         \
        if(cond){\
            retval = (err);          \
            goto label;              \
        }\
    } while (0)

#endif