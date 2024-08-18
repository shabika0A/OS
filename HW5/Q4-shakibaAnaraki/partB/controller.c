#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include "shared_mem.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>

#define MAX_CONSUMERS 10
#define CONSUMER_NAME_SIZE 20
#define MIN_WANTED_STACK_SIZE 20
#define MAX_WANTED_STACK_SIZE 30

int main() {
    int shm_fd;
    SharedStack *shared_stack;
    pid_t consumers[MAX_CONSUMERS];
    char consumer_name[CONSUMER_NAME_SIZE];
    int num_consumers = 0;
    int pid;

    printf("Controller started.\n");

    // Open the shared memory segment
    shm_fd = shm_open(NAME, O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("Failed to open shared memory");
        return EXIT_FAILURE;
    }

    // Map the shared memory segment in the address space of the process
    shared_stack = (SharedStack *)mmap(0, sizeof(SharedStack), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_stack == MAP_FAILED) {
        perror("Failed to map shared memory");
        return EXIT_FAILURE;
    }
    
    // Initially start one consumer
    // pid_t pid = fork();
    // if (pid == 0) {
    //     execl("./consumerRun", "./consumerRun", NULL);  // Adjust as necessary
    //     exit(0);
    // } else if (pid > 0) {
    //     consumers[num_consumers] = pid;
    //     num_consumers+=1;
    //     printf("Initial consumer created, PID: %d\n", pid);
    // } else {
    //     perror("Failed to fork initial consumer");
    //     return EXIT_FAILURE;
    // }

    while (1) {
        int stack_size = shared_stack->top + 1;
        
        if (stack_size > MAX_WANTED_STACK_SIZE && num_consumers < MAX_CONSUMERS || num_consumers==0) {
            pid = fork();
            if (pid == 0) {
                snprintf(consumer_name, CONSUMER_NAME_SIZE, "consumer%d", num_consumers + 1);
                execl("./consumerRun", "consumer", NULL);  // Adjust as necessary
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                consumers[num_consumers++] = pid;
                printf("Consumer added, PID: %d\n", pid);
            } else {
                perror("Failed to fork");
            }
        }

        if (stack_size < MIN_WANTED_STACK_SIZE && num_consumers > 1) {
            pid = consumers[num_consumers];
            num_consumers-=1;
            kill(pid, SIGTERM);
            printf("Consumer removed, PID: %d\n", pid);
        }
        
        printf("Current stack size: %d, Number of consumers: %d\n", stack_size, num_consumers);
        sleep(5);
    }
    
    return 0;
}
