#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

#define MAX_CHILDS 5

int traductor(char file[512], char autor[512]) {
    pid_t tdc = fork();
    if (tdc == 0) {
        if (-1 == execl("./traductor", "0", file, autor, NULL)) {
            perror("execl");
            return -1;
        };
    }
    wait(NULL);
    return 0;
}

int test_one_client() {
    printf("Test 1 sólo cliente:\n");
    int pid = fork();
    if (pid == 0) {
        execl("./cliente", "", NULL);
    }
    wait(NULL);
    return 0;
}

int test_two_writers() {
    printf("Tests 2: 2 escritores\n");

    int pid = fork();
    if (pid != 0) {
        pid = fork();
    }

    if (0 == pid) {
        printf("HIJO\n");
        execl("./custom_client", "0", "3", NULL);
    }
    pid_t last = -1;
    for (int i = 0; i < 2; ++i) {
        last = wait(NULL);
    }
    return last;

}

int test_two_creating_same_file() {
    printf("Tests 3: 2 creando el mismo archivo\n");

    int pid = fork();
    if (pid != 0) {
        pid = fork();
    }

    if (0 == pid) {
        printf("HIJO\n");
        execl("./custom_client", "0", "1", NULL);
    }

    for (int i = 0; i < 2; ++i) {
        wait(NULL);
    }
    return 0;
}

int test_one_reading_one_writing(int wdelay) {
    printf("Tests 4: escritor-lector\n");
    int pid1, pid2;

    pid1 = fork();
    if (pid1 != 0) {
        pid2 = fork();
    }
    else {
        sleep(wdelay);
        execl("./custom_client", "0", "3", NULL);
    }

    if (pid2 == 0) {
        execl("./custom_client", "0", "2", NULL);
    }

    pid_t last = -1;
    for (int i = 0; i < 2; ++i) {
        last = wait(NULL);
    }
    return last;

}

int test_MAX_CHILDS_clients_writing() {
    printf("Tests 2: 2 escritores\n");
    int i = MAX_CHILDS;
    pid_t pid = 1;
    while (i--) {
        if (0 == pid) {
            execl("./custom_client", "0", "3", NULL);
        }
        else {
            pid = fork();
        }
    }
    pid_t last = -1;
    for (int i = 0; i < MAX_CHILDS; ++i) {
        last = wait(NULL);
    }
    return last;
}

int test_MAX_CHILDS_wr() {
    printf("Tests 2: 2 escritores\n");
    int i = MAX_CHILDS;
    pid_t pid = 1;
    while (i--) {
        if (0 == pid) {
            if (i % 2) {
                execl("./custom_client", "0", "2", NULL);
            }
            else {
                execl("./custom_client", "0", "3", NULL);
            }
        }
        else {
            pid = fork();
        }
    }
    pid_t last = -1;
    for (int i = 0; i < MAX_CHILDS; ++i) {
        last = wait(NULL);
    }
    return last;
}

int main(int argc, char* argv[]) {
    int pid = fork();
    if (pid == 0) {
        int fd = open("server_log.txt", O_WRONLY | O_CREAT, 0777);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execl("./servidor", "dummy", "4500", NULL);
    }
    test_one_client(); // comunicaicón correcta
    //setup: creating 0.tuple
    int zero_creat = fork();
    if (zero_creat == 0) {
        printf("SETUP\n");
        execl("./custom_client", "0", "1", NULL);
    }
    else {
        wait(NULL);
    }
    int i = 5;
    pid_t last = 0;
    while (i--) { // se observará la última modificación
        last = test_two_writers(); // en este podremos comprobar la concurrencia 
        //si se da el caso en el que ambos empiecen antes de que el primero termine
    }
    printf("last_writer: Client_%d\n", last);
    char file[512];
    char autor[512];
    sprintf(file, "tuples/%d.tuple", 0);
    sprintf(autor, "test_result-Client_%d.txt", last);
    traductor(file, autor);


    //setup: erasing every file
    int intit_creat = fork();
    if (intit_creat == 0) {
        printf("SETUP: test3\n");
        execl("./custom_client", "0", "0", NULL);
    }
    else {
        wait(NULL);
    }
    test_two_creating_same_file();
    test_one_reading_one_writing(0);
    test_one_reading_one_writing(2);// forzar primero la lectura
    test_MAX_CHILDS_clients_writing();
    test_MAX_CHILDS_wr();

    return 0;
}
