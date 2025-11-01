#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "err.h"
#include "repl.h"
#include "preproc.h"
#include "lexer.h"

int VERBOSE;

/**
 * @brief 
 * 
 * @param source_code 
 * @return int 
 */
int process_code_block(char* source_code){
    int* tok_len;
    char** tok_i;

    preprocess(source_code);
    tokenize(source_code, tok_i, tok_len);
    return ERR_NO_ERROR;
};

/**
 * @brief Entry point of the program - a simple interpret of Lisp language subset
 * handles arguments and either beigns the interpret loop or executes given Lisp source code and exits
 *
 * @param argc count of elements in the argv array
 * @param argv array of argument values
 * @return exit status defined in err.h 
 */
 int main(int argc, char** argv){
    // ran with no args, enter interpret loop
    if (argc == 1){ return repl(); }

    int retval;
    FILE* fptr;
    char* source_code;

    if (argc > 3){ return ERR_INVALID_ARGS; } 
    fptr = fopen(argv[1], "r");
    if(fptr == NULL) { return ERR_INVALID_INPUT_FILE; }

    int verbose = 0;
    if (argc == 3) {
        if(!strcmp("-v", argv[2])) return ERR_INVALID_ARGS;
        verbose = 1;
    }

    if (fseek(fptr, 0, SEEK_END) != 0) { return ERR_INVALID_INPUT_FILE; }
    long file_size = ftell(fptr);
    if (file_size < 0) { return ERR_FILE_ACCESS_FAILURE; }
    rewind(fptr);

    source_code = malloc((size_t)file_size + 1);
    if (!source_code) { return ERR_OUT_OF_MEMORY; }
    size_t n = fread(source_code, 1, (size_t)file_size, fptr);
    if (n != (size_t)file_size) { free(source_code); return ERR_FILE_ACCESS_FAILURE; }
    source_code[file_size] = '\0';

    process_code_block(source_code);
    // int retval = exec_file(fptr, verbose);
    
    // evaluate the file content

    retval = ERR_NO_ERROR;
    cleanup:
    free(source_code);
    if(fptr != NULL) fclose(fptr);
    return retval;
}
