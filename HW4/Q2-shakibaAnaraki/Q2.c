// I used https://www.geeksforgeeks.org/use-posix-semaphores-c/ and gpt 3.5 for checking sintax
// I also runed it on ubuntu in virtual box because I had MacOS which was not compatible with semaphore.h based
// on this: https://stackoverflow.com/questions/27736618/why-are-sem-init-sem-getvalue-sem-destroy-deprecated-on-mac-os-x-and-w

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t A_isPrinted;
sem_t F_isPrinted;

void* p1(void* arg) {
    sem_wait(&A_isPrinted);
    printf("F \n");
    sem_post(&F_isPrinted);
    printf("E \n");
    printf("G \n");
    return NULL;
}

void* p2(void* arg) {
    printf("A \n");
    sem_post(&A_isPrinted);
    sem_wait(&F_isPrinted);
    printf("C \n");
    printf("B \n");
    return NULL;
}

int main() {
    sem_init(&A_isPrinted, 0, 0); 
    sem_init(&F_isPrinted, 0, 0);  

    pthread_t t1, t2;
    pthread_create(&t1, NULL, p1, NULL);
    pthread_create(&t2, NULL, p2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);  

    sem_destroy(&A_isPrinted);
    sem_destroy(&F_isPrinted);

    return 0;
}

