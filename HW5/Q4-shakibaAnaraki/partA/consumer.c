#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "shared_mem.h"
#include <unistd.h>
#include <semaphore.h>

int main() {
    int shm_fd;
    SharedStack *shared_stack;
    sem_t *empty, *full;
    printf("consumer started");

    empty = sem_open(empty_name, O_RDWR, 0666, 0);
    full = sem_open(full_name, O_RDWR, 0666, 0);
    if (empty == SEM_FAILED || full == SEM_FAILED) {
        perror("Failed to open semaphores");
        return EXIT_FAILURE;
    }

    shm_fd = shm_open(NAME, O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("Failed to open shared memory");
        return EXIT_FAILURE;
    }

    shared_stack = (SharedStack *)mmap(0, sizeof(SharedStack), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_stack == MAP_FAILED) {
        perror("Failed to map shared memory");
        return EXIT_FAILURE;
    }
    int i=0;

    while (i<20) {
        sem_wait(full); 

        if (shared_stack->top >= 0) {
            int consumedValue = shared_stack->data[shared_stack->top];
            shared_stack->top--;
            printf("Consumed: %d\n", consumedValue);
        }

        sem_post(empty); 
        i++;
        sleep(3); 
    }

    // Cleanup
    munmap(shared_stack, sizeof(SharedStack));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);

    return EXIT_SUCCESS;
}
