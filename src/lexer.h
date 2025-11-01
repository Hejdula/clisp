#ifndef LEXER
#define LEXER

#include "err.h"

/**
 * @brief Fills token_indices and len_array by tokens from source_code, 
 * splits by " " also separately tokenizes each bracket
 * 
 * @param source_code 
 * @param token_indices Array of pointers into source code for each token
 * @param len_array Lenght of each token
 * @return int 
 */
int tokenize(char* source_code, char** token_indices, int* len_array);

#endif