#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/claves.h"

int main(int argc, char *argv[]) {
    int status;
    int sample_key = 0;
    char sample_str[256];
    sprintf(sample_str, "This file was writen by CLIENT_%d", getpid());
    double sample_vector[32];
    
    for (int i = 0; i < 32; ++i) {
        sample_vector[i] = getpid() + 1/(double)(i + 1);
    }
    int sample_N = 32;

    char Bsample_str[256];
    sprintf(Bsample_str, "This file was modified by CLIENT_%d", getpid());
    double Bsample_vector[32];
    int Bsample_N = 32; 

    for (int i = 0; i < 32; ++i) {
        Bsample_vector[i] = getpid() - 1/(double)(i + 1);
    }

    int received_N = -1; 
    char received_str[256];
    double received_vector[32];


    if (argc != 2) {
        fprintf(stderr, "Error: 2 arguments were expected but %d was given\n", argc);
        return -1; 
    }
    

    int argument = atoi(argv[1]);

    switch (argument)
    {
    case 0:
        status = init();
        break;
    case 1:
        status = set_value(sample_key, sample_str, sample_N, sample_vector);
        break;
    case 2:
        status = get_value(sample_key, received_str, &received_N, received_vector);
        printf("get_value:\n\ttexto: %s\n\tN: %d\n\tdoubles:", received_str, received_N);
        for (int i = 0; i < received_N; ++i) {
            printf("\n\t\t[%d]: %lf", i, received_vector[i]); 
        }
        printf("\n");
        break;
    case 3:
        status = modify_value(sample_key, Bsample_str, Bsample_N, Bsample_vector);
        break;
    case 4:
        status = delete_key(sample_key);
        break;
    case 5:
        status = exist(sample_key);
        break;
    default:
        fprintf(stderr, "ERROR: unrecognised op code (%d)", argument);
        return -1;
    }
    //printf(">>Client_%d end status: %d\n", getpid(), status);

    return 0; 
}