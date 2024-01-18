#include "processes.h"

sem_t *semP1, *semP2, *semP3;

FILE *file;

void Process1() {
    while (1) {
        sem_wait(semP1);
        printf("A");
        fprintf(file, "A");
        fflush(file);
        sem_post(semP3); //Signals Process3
    }
}

void Process2() {
    while (1) {
        sem_wait(semP2);
        printf("B");
        fprintf(file, "B");
        fflush(file);
        sem_post(semP1); //Signals Process1 
    }
}

void Process3() {
    while (1) {
        sem_wait(semP3);
        printf("C");
        fprintf(file, "C");
        fflush(file);
        sem_post(semP2); //Signals Process2
    }
}
