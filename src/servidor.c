#include <stdio.h> 
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "mensajes.h"
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

int tratar_peticion(peticion* p) {
    peticion local_peticion;
    respuesta r;

    pthread_mutex_lock(&mutex);
    
    // copia de la petición 
    local_peticion.key = p->key;
    local_peticion.N_i = p->N_i;
    local_peticion.op = p->op;
    strcpy(local_peticion.q_client, p->q_client);
    strcpy(local_peticion.value1, p->value1);
    copiado = 1;
    pthread_cond_signal(&cond);

    if (p->op == 1 || p->op == 3) // sólo debemos copiar el servidor para el set o modify
    { 
        for (int i = 0; i < local_peticion.N_i; ++i) {
            local_peticion.value2[i] = p->value2[i];
        }
    }    

    pthread_mutex_unlock(&mutex);

    // debug
    printf("starting %s: %d\n", local_peticion.q_client, local_peticion.op);
    //printf("petición:\n\tcliente: %s\n\top: %d\n\tvalue1:%s\n", local_peticion.q_client, local_peticion.op, local_peticion.value1);


    switch (local_peticion.op) {
    case 0: // init
        /* printf("Init\n"); */
        r.success = init();
        break;

    case 1: // set value
        //sleep(10);
        //printf("%s, Set Value\n", local_peticion.q_client);
        
        r.success = set_value(local_peticion.key, local_peticion.value1, local_peticion.N_i, local_peticion.value2);
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

    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(peticion);

    server = mq_open("/SERVIDOR", O_CREAT | O_RDONLY, 0700, &attr);

    double v[] = { -1.0, 3.14, 7.65 };
    char s[] = "Hello world";

    while (!!1) {
        peticion p;
        mq_receive(server, (char*)&p, sizeof(peticion), NULL);
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

