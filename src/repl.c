#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "err.h"
#include "repl.h"
#include "preproc.h"


#define INPUT_BUFF_SIZE 1024

int repl(){
    char* buff = malloc(INPUT_BUFF_SIZE),* line;
    while ((line = fgets(buff, INPUT_BUFF_SIZE, stdin))){
        line[strcspn(line,"\n")] = '\0';
        printf("%s>", line);
    }
    free(buff);
    return ERR_NO_ERROR;
}

