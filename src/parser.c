#include "parser.h"
#include "ast.h"
#include "err.h"
#include "macros.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

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
 * @return err_t error ERR_NO_ERROR on success, otherwise syntax error or out_of_memory
 */
err_t parse_list(astnode **out_node, const char **tokens, int *curr_tok) {
  err_t err, retval;
  astnode *node = NULL;

  RETURN_ERR_IF(!out_node, ERR_INTERNAL);
  *out_node = get_list_node();
  while (tokens[*curr_tok] && tokens[*curr_tok][0] != ')') {
    err = parse_expr(&node, tokens, curr_tok);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
    err = add_child_node(*out_node, node);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
  }
  return ERR_NO_ERROR;
fail_cleanup:
  free_node(*out_node);
  return retval;
};

int is_symbol(const char *s) {
  RETURN_VAL_IF(!s || !*s, 0);
  while (*s) {
    RETURN_VAL_IF(!(isgraph(*s)), 0);
    s++;
  }
  return 1;
}

/**
 * @brief Checks if all characters of string are digits
 * 
 * @param s
 * @return int 1 if number, 0 otherwise
 */
int is_number(const char *s) {
  RETURN_VAL_IF(!s || !*s, 0);
  while (*s) {
    RETURN_VAL_IF(!isdigit(*s), 0);
    s++;
  }
  return 1;
}

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
err_t parse_expr(astnode **out_node, const char **tokens, int *curr_tok) {
  err_t err, retval;
  astnode *quote_symbol_node = NULL, *inner_node = NULL;

  const char *next_token = tokens[*curr_tok];
  /* turns the "'E" into node: (quote <expr>) and parse anything after "'" as expresion*/
  if (next_token[0] == '\'') {
    (*curr_tok)++;
    err = parse_expr(&inner_node, tokens, curr_tok);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);

    *out_node = get_list_node();
    CLEANUP_WITH_ERR_IF(!*out_node, fail_cleanup, ERR_OUT_OF_MEMORY);
    (*out_node)->origin = AST;

    quote_symbol_node = get_symbol_node("quote");
    CLEANUP_WITH_ERR_IF(!quote_symbol_node, fail_cleanup, ERR_OUT_OF_MEMORY);
    quote_symbol_node->origin = AST;

    err = add_child_node(*out_node, quote_symbol_node);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);

    err = add_child_node(*out_node, inner_node);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);

  /* parse a list node surrounded by brackets */
  } else if (next_token[0] == '(') {
    (*curr_tok)++;
    err = parse_list(out_node, tokens, curr_tok);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
    CLEANUP_WITH_ERR_IF(tokens[*curr_tok][0] != ')', fail_cleanup,
                        ERR_SYNTAX_ERROR);
    (*curr_tok)++;

  /* create a number node */
  } else if (is_number(next_token)) {
    (*curr_tok)++;
    char *endptr = NULL;
    int val = strtol(next_token, &endptr, 10);
    CLEANUP_WITH_ERR_IF(*endptr != '\0', fail_cleanup, ERR_SYNTAX_ERROR);
    *out_node = get_number_node(val);
    CLEANUP_WITH_ERR_IF(!*out_node, fail_cleanup, ERR_OUT_OF_MEMORY);
    (*out_node)->origin = AST;

  /* create a symbol node*/
  } else if (is_symbol(next_token)) {
    (*curr_tok)++;
    *out_node = get_symbol_node(next_token);
    CLEANUP_WITH_ERR_IF(!*out_node, fail_cleanup, ERR_OUT_OF_MEMORY);
    (*out_node)->origin = AST;
  
  /* does not match expression defined by grammar */
  } else
    return ERR_SYNTAX_ERROR;

  printf("parsed: %s, returning:", next_token);
  print_node(*out_node);
  printf("\n");
  return ERR_NO_ERROR;

fail_cleanup:
  free_node(quote_symbol_node);
  free_node(inner_node);
  free_node(*out_node);
  return retval;
}
