#ifndef LEXER
#define LEXER

/**
 * @brief Tokenize Lisp-like source code into strings.
 *
 * Splits on spaces (' ') and tabs and treats each of the characters `'`, `(`,
 * `)` as standalone one-character tokens. The input is not modified.
 *
 * On success, allocates a NULL-terminated array of N heap-allocated,
 * NULL-terminated token strings and stores it in *tokens.
 *
 * Its caller responsibility to free each token and the token array on success
 *
 * @param source_code NUL-terminated input string.
 * @param tokens out param; must point to a char** initialized to NULL.
 * @return number of tokens on success; negative err_t on failure.
 */
int tokenize(const char *source_code, char ***tokens);

/**
 * @brief Split preprocessed source into top-level Lisp expressions.
 *
 * @param source_code NUL-terminated preprocessed input string.
 * @param tokens out param; receives NULL-terminated array of expressions.
 * @return number of expressions on success; negative err_t on failure.
 */
int extr_expr(const char *source_code, char ***outer_expr);

#endif