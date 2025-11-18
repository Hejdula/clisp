#include "env.h"
#include "err.h"
err_t process_code_block(char *source_code, int verbose, env* env);
err_t agregate_exit_status(err_t exit_status);
err_t run(int argc, char **argv);