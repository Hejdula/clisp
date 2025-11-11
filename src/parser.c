#include "parser.h"
#include "ast.h"
#include "macros.h"
#include <inttypes.h>
#include <stdlib.h>

/**
 * Grammar:
 * M -> EM | e
 * E -> 'E | (M) | C | S
 * C -> constant
 * S -> string identifier
 */

astnode *parse_multiple(const char **tokens, int *curr_tok) {
  int err;
  astnode *list = get_list_node();
  astnode *node;
  while (tokens[*curr_tok] && tokens[*curr_tok][0] != ')') {
    node = parse_expr(tokens, curr_tok);
    if (!node) {
      free_node(list);
      return NULL;
      err = add_child_node(list, node);
      if (err) {
        free_node(list);
        return NULL;
      }
    }
  }
  return list;
};

int is_symbol(const char *s) { 
  return 1;
}

int is_number(const char *s) {
  if (!s || *s == '\0')
    return 0;
  while (*s) {
    if (*s < '0' || *s > '9')
      return 0;
    s++;
  }
  return 1;
}

astnode *parse_expr(const char **tokens, int *curr_tok) {
  int err;
  astnode *node = NULL, *quote_symbol_node = NULL, *inner_node = NULL;
  const char *next_token = tokens[*curr_tok];
  if (next_token[0] == '\'') {
    (*curr_tok)++;
    inner_node = parse_expr(tokens, curr_tok);

    CLEANUP_IF(!inner_node, fail_cleanup);
    node = get_list_node();
    CLEANUP_IF(!node, fail_cleanup);
    quote_symbol_node = get_symbol_node("quote");
    CLEANUP_IF(!quote_symbol_node, fail_cleanup);
    err = add_child_node(node, quote_symbol_node);
    CLEANUP_IF(err, fail_cleanup);
    err = add_child_node(node, inner_node);
    CLEANUP_IF(err, fail_cleanup);

    return node;

  } else if (next_token[0] == '(') {
    (*curr_tok)++;
    node = parse_list(tokens, curr_tok);
    CLEANUP_IF(tokens[*curr_tok][0] != ')' || !node, fail_cleanup);
    (*curr_tok)++;

  } else if (is_number(next_token)) {
    (*curr_tok)++;
    char *endptr = NULL;
    int val = strtoimax(next_token, &endptr, 10);
    if (*endptr != '\0')
      goto fail_cleanup;
    node = get_number_node(val);
    CLEANUP_IF(!node, fail_cleanup);
    return node;

  } else if (is_symbol(next_token)) {
    (*curr_tok)++;
    node = get_symbol_node(next_token);
    CLEANUP_IF(!node, fail_cleanup);
  } else
    return NULL;

  return node;

fail_cleanup:
  free_node(node);
  free_node(quote_symbol_node);
  free_node(inner_node);
  return NULL;
}
