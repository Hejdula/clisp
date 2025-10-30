#ifndef ERR_H
#define ERR_H

/* Simple project error codes header
 * - Provides `err_t` enum values
 * - Header-only helper `err_str()` as `static inline` so no separate .c is needed
 */

typedef enum {
	ERR_NO_ERROR = 0,
	ERR_INVALID_INPUT_FILE,
	ERR_SYNTAX_ERROR,
	ERR_FILE_ACCESS_FAILURE,
    ERR_OUT_OF_MEMORY,
    ERR_TOO_MANY_ARGUMENTS
} err_t;

/* Returns a static string for the error code. */
static inline const char *err_str(err_t code) {
    switch (code) {
    case ERR_NO_ERROR: return "OK";
    case ERR_INVALID_INPUT_FILE: return "Invalid input file";
    case ERR_SYNTAX_ERROR: return "Syntax error";
    case ERR_FILE_ACCESS_FAILURE: return "File access failure";
    case ERR_OUT_OF_MEMORY: return "Out of memory";
    default: return "Unknown error";
    }
}

#endif