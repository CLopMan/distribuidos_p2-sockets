#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "const.h"
#include "claves.h"
#include "common.h"


/*mqd_t crear_cola(char name[CHAR_SIZE]) {
    //printf("%s\n", name);
    char local_name[CHAR_SIZE];
    strcpy(local_name, name);
    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(respuesta);
    mqd_t queue = mq_open(local_name, O_CREAT | O_RDONLY, 0777, &attr);

    return queue;
}*/

/*int hacer_peticion(char* queue_name, peticion* p, respuesta* r) {
    // servidor
    mqd_t server = mq_open("/SERVIDOR", O_WRONLY);
    if ((mqd_t)-1 == server) {
        perror("Error: cola servidor");
        return -1;
    }

    // cliente
    mqd_t yo = crear_cola(queue_name);
    if ((mqd_t)-1 == yo) {
        perror("Error: cola propia");
        return -1;
    }

    // enviar petición
    if (-1 == mq_send(server, (char*)p, sizeof(peticion), 0)) {
        perror("Error: enviar");
        return -1;
    }

    // recibir petición 
    //printf("recibiendo petición\n");
    if (-1 == mq_receive(yo, (char*)r, sizeof(respuesta), NULL)) {
        perror("Error: recibir");
        return -1;
    }

    mq_close(yo);
    mq_unlink(queue_name);
    return 0;
}*/

int get_client_socket() {
    char *ip;
    char *str_port;
    ip = getenv("IP_TUPLAS");
    if (ip == NULL) {
        perror("NOT setted ip");
        return -1;
    }

    str_port = getenv("PORT_TUPLAS");
    if (str_port == NULL) {
        perror("NOT setted port");
        return -1;
    }
    int port = atoi(str_port);
    int sd = clientSocket(ip, port);
    return sd;
}


int init() {
    int sd = get_client_socket();
    if (-1 == sd) return -1;

    char op[] = "0";
    sendMessage(sd, op, 2);
    char success[32];
    readLine(sd, success, 32);

    //int sd = clientSocket(ip, );
    return atoi(success);


}

int set_value(int key, char* value1, int N_value2, double* V_value2) {
    int sd = get_client_socket();
    if(-1 == sd) return -1;

    char op[] = "1";
    sendMessage(sd, op, 2);

    char strint[CHAR_SIZE];
    sprintf(strint, "%i", key);
    writeLine(sd, strint);

    writeLine(sd, value1);

    sprintf(strint, "%i", N_value2);
    writeLine(sd, strint);

    for (int i = 0; i < N_value2; ++i) {
        sprintf(strint, "%lf", V_value2[i]);
        writeLine(sd, strint);
    }

    char success[32];
    readLine(sd, success, 32);

    return atoi(success);

}

int get_value(int key, char* value1, int* N_value2, double* V_value2) {
    return 1;
    /*peticion p;
    respuesta r;

    char queue_name[CHAR_SIZE];
    sprintf(queue_name, "/Client_%i", getpid());

    // rellenar petición
    p.op = 2;
    p.key = key;

    strcpy(p.value1, value1);
    strcpy(p.q_client, queue_name);

    if (-1 == hacer_peticion(queue_name, &p, &r)) {
        return -1;
    }

    *N_value2 = r.N;
    for (int i = 0; i < r.N; ++i) {
        V_value2[i] = r.value2[i];
    }

    strcpy(value1, r.value1);


    return r.success;*/

}

int modify_value(int key, char* value1, int N_value2, double* V_value2) {
    int sd = get_client_socket();
    if(-1 == sd) return -1;

    char op[] = "3";
    sendMessage(sd, op, 2);

    char strint[CHAR_SIZE];
    sprintf(strint, "%i", key);
    writeLine(sd, strint);

    writeLine(sd, value1);

    sprintf(strint, "%i", N_value2);
    writeLine(sd, strint);

    for (int i = 0; i < N_value2; ++i) {
        sprintf(strint, "%lf", V_value2[i]);
        writeLine(sd, strint);
    }

    char success[32];
    readLine(sd, success, 32);

    return atoi(success);
}

int delete_key(int key) {
    return 1;
    /*
    char queue_name[CHAR_SIZE];
    sprintf(queue_name, "/Client_%i", getpid());

    peticion p;
    respuesta r;

    p.op = 4;
    p.key = key;
    strcpy(p.q_client, queue_name);

    if (-1 == hacer_peticion(queue_name, &p, &r)) {
        perror("Error:");
        return -1;
    }

    return r.success;*/
}

int exist(int key) {
    return 1;
    /*char queue_name[CHAR_SIZE];
    sprintf(queue_name, "/Client_%i", getpid());

    peticion p;
    respuesta r;

    p.op = 5;
    p.key = key;
    strcpy(p.q_client, queue_name);

    if (-1 == hacer_peticion(queue_name, &p, &r)) {
        return -1;
    }

    return r.success;*/

}