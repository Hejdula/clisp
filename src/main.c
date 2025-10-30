#include <stdlib.h>
#include <stdio.h>
#include "err.h"
#include "repl.h"

/**
 * @brief Entry point of the program - a simple interpret of Lisp language subset
 * handles args either beigns the interpret loop or executes given Lisp source code 
 * 
 * @param argc count of elements in the argv array
 * @param argv array of argument values
 * @return exit status
 */
int main(int argc, char** argv){
    if (argc > 3) { return ERR_TOO_MANY_ARGUMENTS; }
    if (argc == 1){ return repl(); }
    printf("hello world");
    return ERR_NO_ERROR;
}