#include <stdlib.h>
#include <stdio.h>
#include "err.h"
#include "repl.h"
#include <errno.h>
#include <string.h>

int VERBOSE;

/**
 * @brief Entry point of the program - a simple interpret of Lisp language subset
 * handles arguments and either beigns the interpret loop or executes given Lisp source code and exits
 *
 * @param argc count of elements in the argv array
 * @param argv array of argument values
 * @return exit status defined in err.h 
 */
int main(int argc, char** argv){
    if (argc > 3){ return ERR_INVALID_ARGS; }
    if (argc == 1){ return repl(); }
 
    FILE* fptr = fopen(argv[1], "r");
    if(fptr == NULL) { return ERR_INVALID_INPUT_FILE; }

    int verbose = 0;
    if (argc == 3) {
        if(!strcmp("-v", argv[2])) return ERR_INVALID_ARGS;
        verbose = 1;
    }

    // int retval =  exec_file(fptr, verbose);

    // evaluate the file content
    fclose(fptr);
    printf("hello world");
    return ERR_NO_ERROR;
}