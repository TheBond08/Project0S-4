#include <semaphore.h>
#include <stdio.h>

//extern specifies that the variables are defined elsewhere
extern sem_t *semP1, *semP2, *semP3;

extern FILE *file;

//Below are the functions for the child processes
void Process1();
void Process2();
void Process3();