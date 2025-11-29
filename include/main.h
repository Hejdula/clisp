
#ifndef MAIN_H
#define MAIN_H

#include "env.h"
#include "err.h"

/**
 * @brief Entry point of the program - a simple interpret of Lisp language
 * subset
 *
 * @param argc count of elements in the argv array
 * @param argv array of argument values
 * @return exit status defined in err.h
 */
int main(int argc, char **argv);

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
err_t run(int argc, char **argv);

/**
 * @brief Tokenizes, parses the source code and evaluates all outer expressions
 * and prints their result
 *
 * @param source_code to interpret
 * @param verbose print results
 * @param env environment for evaluation
 * @return int
 */
err_t process_code_block(char *source_code, int verbose, env *env);

/**
 * @brief Agregates some of internal error codes to predefined exit codes the
 * program should return.
 *
 * @param exit_status to agretate
 * @return err_t
 */
err_t agregate_exit_status(err_t exit_status);

/**
 * @brief Prints usage help for the interpreter to stderr.
 *
 * @param progname Name of the executable (argv[0])
 */
void print_help(const char *progname);
#endif
