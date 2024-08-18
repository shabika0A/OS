// I used TA section 5 codes posix part.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "shared_mem.h"
#include <time.h>

int main() {
    int shm_fd;
    SharedStack *shared_stack;
    sem_t *empty, *full;
    printf("producer started.\n");
    empty = sem_open(empty_name, O_CREAT, 0666, MAX_STACK_SIZE); 
    full = sem_open(full_name, O_CREAT, 0666, 0);
    srand(time(NULL));

    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("Failed to open shared mem");
        return EXIT_FAILURE;
    }

    ftruncate(shm_fd, sizeof(SharedStack));

    shared_stack = (SharedStack *)mmap(0, sizeof(SharedStack), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared_stack == MAP_FAILED) {
        perror("Failed to map shared memory");
        return EXIT_FAILURE;
    }

    shared_stack->top = -1;
    int num;
    int i=0;

    while (1) {
        sem_wait(empty); 

        int num = rand() % 20 + 1;
        if (shared_stack->top < MAX_STACK_SIZE - 1) {
            shared_stack->top++;
            shared_stack->data[shared_stack->top] = num;
            printf("Pushed: %d\n", num);
        }

        sem_post(full); 
        i++;
        sleep(1);
    }

    munmap(shared_stack, sizeof(SharedStack));
    close(shm_fd);
    return EXIT_SUCCESS;
}

