#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    FILE *archivo = fopen(argv[1], "rb");
    close(STDOUT_FILENO);
    int fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0777);
    dup(fd);
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    // Leer los primeros 256 bytes de texto
    char texto[257];
    fread(texto, 1, 256, archivo);
    texto[256] = '\0';
    printf("Texto: %s\n", texto);

    // Leer el entero de 4 bytes
    int32_t entero;
    fread(&entero, sizeof(int32_t), 1, archivo);
    printf("Entero: %d\n", entero);

    // Leer los 32 valores de punto flotante de doble precisión
    double valores[32];
    fread(valores, sizeof(double), 32, archivo);

    printf("Valores de punto flotante:\n");
    for (int i = 0; i < entero; ++i) {
        printf("\t%lf\n", valores[i]);
    }

    fclose(archivo);
    return 0;
}

