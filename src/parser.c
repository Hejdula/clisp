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
 * M -> EM | e
 * E -> 'E | (M) | C | S
 * C -> constant
 * S -> string identifier
 */

int parse_list(astnode **out_node, const char **tokens, int *curr_tok) {
  RETURN_ERR_IF(!out_node, ERR_INTERNAL);
  int err, retval;
  *out_node = get_list_node();
  astnode *node = NULL;
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
  printf("symbol: %s", s);
  RETURN_VAL_IF(!s || !*s, 0);
  while (*s) {
    RETURN_VAL_IF(!(isgraph(*s)), 0);
    s++;
  }
  return 1;
}

int is_number(const char *s) {
  RETURN_VAL_IF(!s || !*s, 0);
  while (*s) {
    RETURN_VAL_IF(!isdigit(*s), 0);
    s++;
  }
  return 1;
}

int parse_expr(astnode **out_node, const char **tokens, int *curr_tok) {
  int err, retval;
  astnode *quote_symbol_node = NULL, *inner_node = NULL;
  const char *next_token = tokens[*curr_tok];
  if (next_token[0] == '\'') {
    (*curr_tok)++;
    err = parse_expr(&inner_node, tokens, curr_tok);

    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
    *out_node = get_list_node();
    CLEANUP_WITH_ERR_IF(!*out_node, fail_cleanup, ERR_OUT_OF_MEMORY);
    quote_symbol_node = get_symbol_node("quote");
    CLEANUP_WITH_ERR_IF(!quote_symbol_node, fail_cleanup, ERR_OUT_OF_MEMORY);
    err = add_child_node(*out_node, quote_symbol_node);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
    err = add_child_node(*out_node, inner_node);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);

  } else if (next_token[0] == '(') {
    (*curr_tok)++;
    err = parse_list(out_node, tokens, curr_tok);
    CLEANUP_WITH_ERR_IF(err, fail_cleanup, err);
    CLEANUP_WITH_ERR_IF(tokens[*curr_tok][0] != ')', fail_cleanup,
                        ERR_SYNTAX_ERROR);
    (*curr_tok)++;

  } else if (is_number(next_token)) {
    (*curr_tok)++;
    char *endptr = NULL;
    int val = strtol(next_token, &endptr, 10);
    CLEANUP_WITH_ERR_IF(*endptr != '\0', fail_cleanup, ERR_SYNTAX_ERROR);
    *out_node = get_number_node(val);
    CLEANUP_WITH_ERR_IF(!*out_node, fail_cleanup, ERR_OUT_OF_MEMORY);

  } else if (is_symbol(next_token)) {
    (*curr_tok)++;
    *out_node = get_symbol_node(next_token);
    CLEANUP_IF(!*out_node, fail_cleanup);
  } else
    return ERR_SYNTAX_ERROR;

  printf("parsing %s, returning:", next_token);
  print_node(*out_node);
  printf("\n");
  return ERR_NO_ERROR;

fail_cleanup:
  free_node(quote_symbol_node);
  free_node(inner_node);
  free_node(*out_node);
  return retval;
}
