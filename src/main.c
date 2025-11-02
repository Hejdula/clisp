#include "err.h"
#include "lexer.h"
#include "macros.h"
#include "preproc.h"
#include "repl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int DBG_VERBOSE = 1;

int process_code_block(char *source_code, int verbose);

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
  // ran with no args, enter interpret loop
  if (argc == 1)
    return repl();

  int retval, verbose = 0;
  long temp;
  FILE *fptr;
  char *source_code;

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
int process_code_block(char *source_code, int verbose) {
  char **tokens = NULL;
  int *tok_len = NULL;
  

  preprocess(source_code);
  printf("%s", source_code);
  tokenize(source_code, tokens, tok_len);
  if (verbose) verbose = 0;

  free(tokens);
  free(tok_len);
  return ERR_NO_ERROR;
};