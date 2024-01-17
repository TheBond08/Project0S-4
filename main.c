#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *semP1, *semP2, *semP3;
FILE *file;

void Process1(){
    while (1){
        sem_wait(semP1);
        printf("A");
        fprintf(file,"A");
        fflush(file);
        sem_post(semP3);
    }
}

void Process2(){
    while(1){
        sem_wait(semP2);
        printf("B");
        fprintf(file,"B");
        fflush(file);
        sem_post(semP1);
    }
}

void Process3(){
    while(1){
        sem_wait(semP3);
        printf("C");
        fprintf(file,"C");
        fflush(file);
        sem_post(semP2);
    }
}

int main(){

    semP1 = sem_open("semP1", O_CREAT, 0644, 0);
    semP2 = sem_open("semP2", O_CREAT, 0644, 0);
    semP3 = sem_open("semP3", O_CREAT, 0644, 0);


    file = fopen("file.txt", "w");
    if(file == NULL){
        printf("Error opening file");
        exit(1);
    }

    pid_t pid1, pid2, pid3; //Preparing the 3 child processes

    pid1 = fork();
    if(pid1 > 0){
        Process1();
        exit(0);
    } else if(pid1 < 0){
        printf("Failed to fork process 1");
        exit(2);
    }

    pid2 = fork();
    if(pid2 > 0){
        Process2();
        exit(0);
    } else if(pid2 < 0){
        printf("Failed to fork process 2");
        exit(2);
    }

    pid3 = fork();
    if(pid3 > 0){
        Process3();
        exit(0);
    } else if(pid3 < 0){
        printf("Failed to fork process 3");
        exit(2);
    }


    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);


    sem_close(semP1);
    sem_close(semP2);
    sem_close(semP3);
    sem_unlink("semP1");
    sem_unlink("semP2");
    sem_unlink("semP3");    

    fclose(file);

    return 0;
}
