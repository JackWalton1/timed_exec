#include <stdio.h>


int main(int argc, char *argv[]){

    fprintf(stderr, "Here is the second program that prints cmd line args!\n");
    for(int i =0; i<argc; i++){
        fprintf(stderr, "Argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
