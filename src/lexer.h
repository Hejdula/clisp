#ifndef LEXER
#define LEXER

#include "err.h"

int tokenize(char *source_code, int **token_indices, int **len_array);

#endif