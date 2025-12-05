#include "main.h"
#include "ast.h"
#include "env.h"
#include "err.h"
#include "lexer.h"
#include "macros.h"
#include "parser.h"
#include "preproc.h"
#include "repl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Entry point of the program - a simple interpret of Lisp language
 * subset
 *
 * @param argc count of elements in the argv array
 * @param argv array of argument values
 * @return exit status defined in err.h
 */
int main(int argc, char **argv) {
  return agregate_exit_status(run(argc, argv));
  // return run(argc, argv);
}

/**
 * @brief Handles arguments and either beigns the interpret loop or evaluates
 * given Lisp source code and exits
 *
 * The arguments are hardcoded, because so far if there are 2 or 3 args, they
 * can only have one meaning
 *
 * @param argc count of elements in the argv array
 * @param argv array of argument values
 * @return exit status defined in err.h
 */
err_t run(int argc, char **argv) {

  // ran with no args, enter interpret loop
  if (argc == 1)
    return repl();

  int retval, verbose = 0;
  long temp;
  FILE *fptr = NULL;
  char *source_code = NULL;
  env *env = NULL;

  // expect no more than two arguments
  if (argc > 3) {
    print_help(argv[0]);
    return ERR_INVALID_ARGS;
  }

  // with 2 args, the second should be '-v' flag
  if (argc == 3) {
    if (strcmp("-v", argv[2]) != 0) {
      print_help(argv[0]);
      return ERR_INVALID_ARGS;
    }
    verbose = 1;
  }

  // open and read file input into source_code
  fptr = fopen(argv[1], "r");
  RETURN_ERR_IF(!fptr, ERR_INVALID_INPUT_FILE);

  temp = fseek(fptr, 0, SEEK_END);
  CLEANUP_WITH_ERR_IF(temp, cleanup, ERR_FILE_ACCESS_FAILURE);

  temp = ftell(fptr);
  CLEANUP_WITH_ERR_IF(temp < 0, cleanup, ERR_FILE_ACCESS_FAILURE);
  size_t file_size = temp;
  rewind(fptr);

  source_code = malloc(file_size + 1);
  CLEANUP_WITH_ERR_IF(!source_code, cleanup, ERR_OUT_OF_MEMORY);

  size_t n = fread(source_code, 1, file_size, fptr);
  CLEANUP_WITH_ERR_IF(n != file_size, cleanup, ERR_FILE_ACCESS_FAILURE);

  source_code[file_size] = '\0';

  env = create_env();
  RETURN_ERR_IF(!env, ERR_OUT_OF_MEMORY);

  retval = process_code_block(source_code, verbose, env);

cleanup:
  free_env(env);
  free(source_code);
  if (fptr)
    fclose(fptr);
  return retval;
}

/**
 * @brief Tokenizes, parses the source code and evaluates all outer expressions
 * and prints their result
 *
 * @param source_code to interpret
 * @return int
 */
err_t process_code_block(char *source_code, int verbose, env *env) {
  /* sanity check */
  RETURN_ERR_IF(!source_code || !env, ERR_INTERNAL);

  char **tokens = NULL;
  int token_count = 0, i;
  err_t err, retval = ERR_NO_ERROR;

  err = preprocess(source_code);
  RETURN_ERR_IF(err, err);

  token_count = tokenize(source_code, &tokens);
  RETURN_ERR_IF(token_count < 0, -token_count);

  int curr_tok = 0;
  astnode *root = NULL, *result_node = NULL;
  err = parse_list(&root, (const char **)tokens, &curr_tok);
  CLEANUP_WITH_ERR_IF(curr_tok != token_count, cleanup, ERR_SYNTAX_ERROR);
  RETURN_VAL_IF(err, err);
  // print_node(root);
  // printf("\n");

  for (i = 0; i < root->as.list.count; i++) {
    err = eval_node(root->as.list.children[i], &result_node, env);
    if (err == CONTROL_BREAK)
      err = ERR_SYNTAX_ERROR;

    CLEANUP_WITH_ERR_IF(err, cleanup, err);
    if (verbose) {
      printf("[%d]> ", i + 1);
      print_node(root->as.list.children[i]);
      printf("\r\n");
      print_node(result_node);
      printf("\r\n");
    }
    free_temp_node_parts(result_node);
    result_node = NULL;
  }

cleanup:
  for (i = 0; i < token_count; i++) {
    // printf("token %d: %s\n", i, tokens[i]);
    free(tokens[i]);
  }
  free_node(root);
  free(tokens);

  return retval;
};

/**
 * @brief Agregates some of internal error codes to predefined exit codes the
 * program should return.
 *
 * @param exit_status to agretate
 * @return err_t
 */
err_t agregate_exit_status(err_t exit_status) {
  switch (exit_status) {
    case ERR_RUNTIME_UNKNOWN_VAR:
    case ERR_NOT_A_VARIABLE:
    case ERR_UNKNOWN_OPERATOR:
      return ERR_SYNTAX_ERROR;
    default:
      return exit_status;
  }
}

/**
 * @brief Prints usage help for the interpreter to stderr.
 *
 * @param progname Name of the executable (argv[0])
 */
void print_help(const char *progname) {
  fprintf(stderr, "Usage: %s [file] [-v]\n", progname);
  fprintf(stderr, "  file   Lisp source file to interpret\n");
  fprintf(stderr, "  -v     (optional) print results of all expressions\n");
}