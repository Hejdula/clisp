#include "ast.h"
#include "err.h"

#ifndef PARSER_H
#define PARSER_H

/**
 * Grammar:
 * L(list) -> EL | e
 * E(expression) -> 'E | (L) | C | S
 * C -> constant
 * S -> string identifier
 */

/**
 * @brief Parser for grammar rule: "L -> EL | e"
 * Calls parse_expr on each token until empty/NULL token or ')'
 * (The ')' coming from rule E -> (L))
 *
 * Expects no tokens to be NULL except the last one and "'", "(", ")" to be
 * separate tokens
 *
 * @param out_node root of resulting AST, NULL on failure
 * @param tokens array of all tokens to process
 * @param curr_tok index into tokens pointing to current token to parse
 * @return err_t error ERR_NO_ERROR on success, otherwise syntax error or
 * out_of_memory
 */
err_t parse_list(astnode **out_node, const char **tokens, int *curr_tok);

/**
 * @brief Parser for grammar rule: "E -> 'E | (L) | C | S"
 *
 * Decides form by current token and builds AST:
 *  - 'E  -> (quote <expr>)
 *  - (L) -> parse_list(...) until ')'
 *  - C   -> number node (digits only)
 *  - S   -> symbol node (printable token)
 *
 * Advances curr_tok past the whole expression. Expects "'", "(", ")"
 * to be separate tokens.
 *
 * @param out_node root of resulting AST, NULL on failure
 * @param tokens array of all tokens to process
 * @param curr_tok index into tokens pointing to current token
 * @return err_t ERR_NO_ERROR on success, otherwise syntax/out_of_memory
 */
err_t parse_expr(astnode **out_node, const char **tokens, int *curr_tok);

#endif