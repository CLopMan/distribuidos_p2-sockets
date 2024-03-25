#include "imp_clave.h"
#include "const.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

/*Estructura de fichero
char [256]
N_value [4]
double[32*8]
*/

int init() {
    DIR* dir;
    struct dirent* entrada;

    dir = opendir("tuples");

    if (dir == NULL) {
        if (-1 == mkdir("tuples", 0700)) {
            perror("Error al crear el directorio:");
            return -1;
        }
    }
    else {
        while ((entrada = readdir(dir)) != NULL) {
            if (entrada->d_type == DT_REG) {  // Si es un archivo regular
                char ruta_archivo[1024];
                snprintf(ruta_archivo, sizeof(ruta_archivo), "%s/%s", "tuples", entrada->d_name);

                if (remove(ruta_archivo) == 0) {
                    printf("Se ha eliminado el archivo: %s\n", ruta_archivo);
                }
                else {
                    perror("Error al eliminar el archivo");
                }
            }
        }
    }

    closedir(dir);

}

int write_file(int key, char* value1, int N_value2, double* V_value2, char permiso[4]) {

    char file_name[32];
    sprintf(file_name, "./tuples/%i.tuple", key);

    FILE* tuple = fopen(file_name, permiso); // O_TRUNC, O_WRONLY, O_CREAT if exists return an error
    // dos hilos no pueden entrar aquí a la vez con el mismo valor de clave
    if (NULL == tuple) {
        perror("ERROR-fopen:");
        return -1;
    }

    if (-1 == fwrite(value1, 1, 256, tuple)) {
        perror("ERROR-write value 1");
        return -1;
    }

    if (-1 == fwrite(&N_value2, sizeof(int), 1, tuple)) {
        perror("ERROR-write N_value:");
        return -1;
    }

    if (-1 == fwrite(V_value2, sizeof(double), N_value2, tuple)) {
        perror("ERROR-write doubles: ");
        return -1;
    }
    fclose(tuple);
    return 0;
}

// la he probado con un minicódigo en c y parece que funciona
int set_value(int key, char* value1, int N_value2, double* V_value2) {
    /*validaciones*/
    if (N_value2 > 32 || N_value2 < 1) {
        perror("ERROR: N_value 2 out of range");
        return -1;
    }

    return write_file(key, value1, N_value2, V_value2, "wx\0");
}

int modify_value(int key, char* value1, int N_value2, double* V_value2) {
    /*validaciones*/
    if (N_value2 > 32 || N_value2 < 1) {
        perror("ERROR: N_value 2 out of range");
    }

    return write_file(key, value1, N_value2, V_value2, "r+\0");
}

int get_value(int key, char* value1, int* N_value2, double* V_value2) {
    // Abrir el fichero si existe
    char filename[20];
    if (-1 == sprintf(filename, "./tuples/%i.tuple", key)) {
        perror("Error formateando el nombre del fichero:");
        return -1;
    }

    FILE* desc = fopen(filename, "r");
    if (NULL == desc) {
        perror("Error abriendo el archivo:");
        return -1;
    }
    if (-1 == fread((void*)value1, 1, CHAR_SIZE, desc)) {
        perror("Error leyendo valor1:");
        return -1;
    }

    if (-1 == fread(N_value2, sizeof(int), 1, desc)) {
        perror("ERROR-write N_value:");
        return -1;
    }
    if (-1 == fread(V_value2, sizeof(double), *N_value2, desc)) {
        perror("ERROR-write doubles: ");
        return -1;
    }
    fclose(desc);
    return 0;
}

int delete_key(int key) {
    char filename[20];
    if (-1 == sprintf(filename, "./tuples/%i.tuple", key)) {
        perror("Error formateando el nombre del fichero:");
        return -1;
    }
    unlink(filename);
}

int exist(int key) {
    char filename[20];
    if (-1 == sprintf(filename, "./tuples/%i.tuple", key)) {
        perror("Error formateando el nombre del fichero:");
        return -1;
    }
    FILE* desc = fopen(filename, "r");
    if (NULL != desc) {
        fclose(desc);
        return 1;
    }
    return 0;

}




