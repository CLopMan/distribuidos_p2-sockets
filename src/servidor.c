#include <stdio.h> 
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "const.h"
#include "common.h"
#include "imp_clave.h"

int i = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
int copiado = 0;
mqd_t server;
void stop_server() {
    pthread_mutex_destroy(&mutex);
    printf("Cerrando servidor...\n");
    mq_close(server);
    mq_unlink("/SERVIDOR");
    exit(0);
}

int tratar_peticion(void* sc) {
    int local_sc;

    int success;
    int op;
    int N_i;
    int key;
    char value1[CHAR_SIZE];
    int N_value2;
    double value2[ARR_SIZE];

    char buffer_local[CHAR_SIZE];

    pthread_mutex_lock(&mutex);
    // copia de la petición 
    local_sc = *((int*)sc);
    copiado = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    //printf("petición:\n\tcliente: %s\n\top: %d\n\tvalue1:%s\n", local_peticion.q_client, local_peticion.op, local_peticion.value1);
    readLine(local_sc, buffer_local, sizeof(char));
    op = buffer_local[0] - '0';

    switch (op) {
    case 0: // init
        success = init();
        sprintf(buffer_local, "%i", success);
        writeLine(local_sc, buffer_local);
        break;

    case 1: // set value
        // Leemos key
        readLine(local_sc, buffer_local, CHAR_SIZE);
        key = atoi(buffer_local);

        // Leemos value1
        readLine(local_sc, buffer_local, CHAR_SIZE);
        strcpy(value1, buffer_local);

        // Leemos N_i
        readLine(local_sc, buffer_local, CHAR_SIZE);
        N_i = atoi(buffer_local);

        // Leemos value2
        readLine(local_sc, );

        success = set_value(local_peticion.key, local_peticion.value1, local_peticion.N_i, local_peticion.value2);
        break;

    case 2: // get value
        //sleep(5);
        //printf("%s, Get Value\n", local_peticion.q_client);
        r.success = get_value(local_peticion.key, r.value1, &r.N, r.value2);
        break;

    case 3: // modify value
        /* printf("Modify Value\n"); */

        r.success = modify_value(local_peticion.key, local_peticion.value1, local_peticion.N_i, local_peticion.value2);
        break;

    case 4: // delete key
        /* printf("Delete Key\n"); */
        r.success = delete_key(local_peticion.key);
        break;

    case 5: // exists
        /* printf("Exists\n"); */
        r.success = exist(local_peticion.key);
        break;

    default:
        fprintf(stderr, "Not recognised operation: expected [0, 5] but %d was received\n", local_peticion.op);
        break;
    }
    mqd_t client = mq_open(local_peticion.q_client, O_WRONLY);
    mq_send(client, (char*)&r, sizeof(respuesta), 0);
    printf("finish: %s\n", local_peticion.q_client);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    signal(SIGINT, stop_server);
    int sd, sc;
    if (argc != 2) {
        perror("Uso: ./servidor <puerto>");
        return -1;
    }

    sd = serverSocket(INADDR_ANY, atoi(argv[1]), SOCK_STREAM);

    if (sd < 0) {
        printf("SERVER: Error en serverSocket\n");
        return 0;
    }
    while (!!1) {
        sc = serverAccept(sd);
        if (sc < 0) {
            printf("Error en serverAccept\n");
            continue;
        }
        pthread_t hilo;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&hilo, &attr, (void*)tratar_peticion, (void*)&p);
        pthread_mutex_lock(&mutex);
        while (!copiado) {
            pthread_cond_wait(&cond, &mutex);
        }
        copiado = 0;
        pthread_mutex_unlock(&mutex);

    }

    return 0;

}

