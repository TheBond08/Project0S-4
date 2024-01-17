#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
//#include "processes.h"

#define ERROR_ITINIALIZING_SEMAPHORE 1
#define ERROR_CANNOT_OPEN_FILE 2
#define ERROR_FORK_PROCESS_FAILED 3

sem_t *semP1, *semP2, *semP3;
FILE *file;

void Process1() {
    while (1) {
        sem_wait(semP1);
        printf("A");
        fprintf(file, "A");
        fflush(file);
        sem_post(semP2);
    }
}

void Process2() {
    while (1) {
        sem_wait(semP2);
        printf("B");
        fprintf(file, "B");
        fflush(file);
        sem_post(semP3);
    }
}

void Process3() {
    while (1) {
        sem_wait(semP3);
        printf("C");
        fprintf(file, "C");
        fflush(file);
        sem_post(semP1);
    }
}

int main() {


    sem_unlink("semP1");
    sem_unlink("semP2");
    sem_unlink("semP3");

    //Initializing the semaphores
    semP1 = sem_open("semP1", O_CREAT, 0644, 0);
    semP2 = sem_open("semP2", O_CREAT, 0644, 1);
    semP3 = sem_open("semP3", O_CREAT, 0644, 0);

    if (semP1 == SEM_FAILED || semP2 == SEM_FAILED || semP3 == SEM_FAILED) { //Checking if the semaphores were initialized successfully
        perror("Semaphore initialization failed\n");
        return ERROR_ITINIALIZING_SEMAPHORE;
    }

    //Opening the file to write the results
    file = fopen("file.txt", "w");
    printf("Opening file\n");
    if (file == NULL) {
        printf("Error opening file\n");
        return ERROR_CANNOT_OPEN_FILE;
    }

    //Preparing the child processes
    pid_t pid1, pid2, pid3;

    printf("Creating child processes\n");

    pid1 = fork();
    if (pid1 == 0) {
        Process1();
        exit(0);
    } else if (pid1 < 0) {
        printf("Failed to fork process 1\n");
        return ERROR_FORK_PROCESS_FAILED;
    }

    pid2 = fork();
    if (pid2 == 0) {
        Process2();
        exit(0);
    } else if (pid2 < 0) {
        printf("Failed to fork process 2\n");
        return ERROR_FORK_PROCESS_FAILED;
    }

    pid3 = fork();
    if (pid3 == 0) {
        Process3();
        exit(0);
    } else if (pid3 < 0) {
        printf("Failed to fork process 3\n");
        return ERROR_FORK_PROCESS_FAILED;
    }

    //Waiting for the child processes to terminate
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    //Destroying the semaphores
    sem_close(semP1);
    sem_close(semP2);
    sem_close(semP3);
    sem_unlink("semP1");
    sem_unlink("semP2");
    sem_unlink("semP3");

    fclose(file);

    return 0;
}