#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include "claves.h"

int main(int argc, char* argv[]) {
    double tres[] = { 3.3,33.3,333.3 };
    int N;
    char cd[] = "cd";
    char texto[256];
    int rn;
    double rvec[32];
    char hello[] = "Hello world";
    double vec5[] = {1.1, 2.2, 3.3, 4.4, 5.5};

    printf("INIT:\n");
    init();
    sleep(1);

    int retorno = -1; 
    retorno = set_value(3, cd, 3, tres);
    printf("first set: %d\n", retorno); 
    sleep(1);
    retorno = set_value(9, cd, 3, tres);
    printf("snd set: %d\n", retorno);
    sleep(1);


    get_value(3, texto, &rn, rvec);
    printf("get_value:\n\ttexto: %s\n\tN: %d\n\tdoubles:", texto, rn);
    for (int i = 0; i < rn; ++i) {
        printf("\n\t\t[%d]: %lf", i, rvec[i]); 
    }
    printf("\n");

    sleep(1);
    
    modify_value(3, hello, 5, vec5);
    get_value(3, texto, &rn, rvec);
    printf("first_modify:\n\ttexto: %s\n\tN: %d\n\tdoubles:", texto, rn);
    for (int i = 0; i < rn; ++i) {
        printf("\n\t\t[%d]: %lf", i, rvec[i]); 
    }
    printf("\n");
    retorno = modify_value(5, cd, 3, tres);
    printf("snd modify: %d\n", retorno);
    
    delete_key(3);
    printf("exits: %i\n", exist(9));
    printf("exits: %i\n", exist(3));
    return 0;
}