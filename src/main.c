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

err_t process_code_block(char *source_code, int verbose);
err_t agregate_exit_status(err_t exit_status);
err_t run(int argc, char **argv);

/**
 * @brief Entry point of the program - a simple interpret of Lisp language
 * subset handles arguments and either beigns the interpret loop or evaluates
 * given Lisp source code and exits
 *
 * @param argc count of elements in the argv array
 * @param argv array of argument values
 * @return exit status defined in err.h
 */
int main(int argc, char **argv) {
  return agregate_exit_status(run(argc, argv));
}

err_t run(int argc, char **argv) {
  // ran with no args, enter interpret loop
  if (argc == 1)
    return repl();

  int retval, verbose = 1;
  long temp;
  FILE *fptr = NULL;
  char *source_code = NULL;

  // expect no more than two arguments
  RETURN_ERR_IF((argc > 3), ERR_INVALID_ARGS);

  // with 2 args, the second should be '-v' flag
  if (argc == 3) {
    RETURN_ERR_IF(!strcmp("-v", argv[2]), ERR_INVALID_ARGS);
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

  retval = process_code_block(source_code, verbose);

cleanup:
  free(source_code);
  if (fptr)
    fclose(fptr);
  return retval;
}

/**
 * @brief
 *
 * @param source_code
 * @return int
 */
err_t process_code_block(char *source_code, int verbose) {
  char **tokens = NULL;
  int token_count, i, err, retval = ERR_NO_ERROR;

  preprocess(source_code);
  token_count = tokenize(source_code, &tokens);
  int curr_tok = 0;
  astnode *root = NULL, *result_node = NULL;
  err = parse_list(&root, (const char **)tokens, &curr_tok);
  RETURN_VAL_IF(err, err);
  // print_node(root);
  // printf("\n");

  env *env = create_env();
  RETURN_ERR_IF(!env, ERR_OUT_OF_MEMORY);

  for (i = 0; i < root->as.list.count; i++) {
    if (verbose) {
      print_node(root->as.list.children[i]);
      printf(" -> ");
    }
    err = eval_node(root->as.list.children[i], &result_node, env);
    CLEANUP_WITH_ERR_IF(err, cleanup, err);
    if (verbose) {
      print_node(result_node);
      printf("\n");
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
  free_env(env);

  return retval;
};

err_t agregate_exit_status(err_t exit_status) {
  if (exit_status == ERR_RUNTIME_UNKNOWN_VAR ||
      exit_status == ERR_NOT_A_VARIABLE) {
    return ERR_SYNTAX_ERROR;
  }
  return exit_status;
}