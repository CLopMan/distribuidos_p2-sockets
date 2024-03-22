/*Archivo de pruebas de la implementación de las funciones del lado del servidor*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../src/imp_clave.h"

#define TEST_TUPLE "tuples/0.tuple"

int main (int argc, char **argv) {
    
    if (argc != 2) {
        fprintf(stderr, "Error: expected 2 arguments but %d were given\nUssage: ./test_imp <op_code>\n", argc);
        return 2;
    }
    
    int status;
    int sample_key = 0;
    char sample_str[256];
    strcpy(sample_str, "HELLO WORLD");
    double sample_vector[] = {3.14, 2.718, 1.618, 7.98e300};
    int sample_N = 4;

    char Bsample_str[] = "BYE WORLD";
    double Bsample_vector[] = {6.28, 5.418, 2.18};
    int Bsample_N = 3; 

    int received_N = -1; 
    char received_str[256];
    double received_vector[32];

    FILE *zero;

    switch (atoi(argv[1]))
    {
    case 0:
        status = init();
        break;
    case 1:
        //creating newfile
        status = set_value(sample_key, sample_str, sample_N, sample_vector);
        zero = fopen(TEST_TUPLE, "r");
        if (zero == NULL) {
            perror("ERROR:");
            return -1;    
        }
        fread(received_str, 256, 1, zero);

        if (memcmp(sample_str, received_str, strlen(received_str)) != 0) {
            fprintf(stderr, "Error: received string doesn't match expected\n>%s\n<%s\n", sample_str, received_str);
            return -1;
        }

        fread(&received_N, sizeof(int), 1, zero);
        
        if (received_N != sample_N) {
            fprintf(stderr, "Error: received N doesn't match expected\n>%d\n<%d\n", sample_N, received_N);
            return -1;
        }

        fread(received_vector, sizeof(double), received_N, zero);

        if (memcmp(sample_vector, received_vector, sizeof(double)*received_N) != 0) {
            fprintf(stderr, "Error: received vector doesn't match expected\n");
            return -1;
        }

        if (status != 0) {
            fprintf(stderr, "Error: return value was %d when 0 was spected\n", status);
            return -1;
        }

        // creating an existing file. Error expected
        status = set_value(sample_key, sample_str, sample_N, sample_vector);
        if(status != -1) {
            fprintf(stderr, "Error: return value was %d when -1 was spected\n", status);
            return -1;
        }
        fclose(zero);

        break;
    case 2:
        // creating file 
        zero = fopen("tuples/1.tuple", "w");
        if (zero == NULL) {printf("error: creando archivo");}
        int n = fwrite(sample_str, 1, 256, zero);
        fwrite(&sample_N, sizeof(int), 1, zero);
        fwrite(sample_vector, sizeof(double), sample_N, zero);

        strcpy(received_str,"");
        fclose(zero);

        status = get_value(1, received_str, &received_N, received_vector);
        if (strcmp(received_str, sample_str) != 0) {
            fprintf(stderr, "Error: received string doesn't match expected\n>%s\n<%s\n", sample_str, received_str);
            return -1;
        }
        if (received_N != sample_N) {
            fprintf(stderr, "Error: received N doesn't match expected\n>%d\n<%d\n", sample_N, received_N);
            return -1;
        }
        if (memcmp(sample_vector, received_vector, sizeof(double)*received_N) != 0) {
            fprintf(stderr, "Error: received vector doesn't match expected\n");
            return -1;
        }
        if (status != 0) {
            fprintf(stderr, "Error: return value was %d when 0 was spected\n", status);
            return -1;
        }
        // non existing key
        status = get_value(2, received_str, &received_N, received_vector); 
        if (status != -1 ) {
            fprintf(stderr, "Error: return value was %d when -1 was spected\n", status);
            return -1;
        }
        break;
    case 3:
        status = modify_value(sample_key, Bsample_str, Bsample_N, Bsample_vector);
        zero = fopen(TEST_TUPLE, "r");
        if (zero == NULL) {
            perror("ERROR:");
            return -1;    
        }
        fread(received_str, 256, 1, zero);

        if (memcmp(Bsample_str, received_str, strlen(received_str)) != 0) {
            fprintf(stderr, "Error: received string doesn't match expected\n>%s\n<%s\n", Bsample_str, received_str);
            return -1;
        }

        fread(&received_N, sizeof(int), 1, zero);
        
        if (received_N != Bsample_N) {
            fprintf(stderr, "Error: received N doesn't match expected\n>%d\n<%d\n", Bsample_N, received_N);
            return -1;
        }

        fread(received_vector, sizeof(double), received_N, zero);

        if (memcmp(Bsample_vector, received_vector, sizeof(double)*received_N) != 0) {
            fprintf(stderr, "Error: received vector doesn't match expected\n");
            return -1;
        }

        if (status != 0) {
            fprintf(stderr, "Error: return value was %d when 0 was spected\n", status);
            return -1;
        }

        // creating an non-existng file. Error expected
        status = modify_value(2, Bsample_str, Bsample_N, Bsample_vector);
        if(status != -1) {
            fprintf(stderr, "Error: return value was %d when -1 was spected\n", status);
            return -1;
        }
        break;
    case 4:
        status = exist(0);
        if (status != 1) {
            perror("Error: the file exists but it was evaluated as non-existing\n");
            return -1;
        }
        status = exist(2);
        if (status != 0) {
            perror("Error: the file does not exists but it was evaluated as existing\n");
            return -1;
        }
        break;
    case 5:
        status = delete_key(sample_key);
        zero = fopen(TEST_TUPLE, "r");
        if (zero != NULL) {
            perror("Error: the file was not erased");
            fclose(zero);

            return -1;
        }

        // deleting non-existing file 
        status = delete_key(2);
        if(status != -1) {
            fprintf(stderr, "Error: return value was %d when -1 was spected\n", status);
            return -1;
        }
        break;
    default:
        fprintf(stderr, "ERROR: unrecognised op code (%d)", atoi(argv[1]));
        return -1;
    }
    

    return 0; 
}
