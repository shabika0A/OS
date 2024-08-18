#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SIZE 1024
#define NAME "my_shr_mem"

int main() {
    int shm_fd;
    int m = 5, n = 4, k = 3;
    int (*ptr)[k]; // Pointer to a 2D array

    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("Failed to open shared mem");
        return EXIT_FAILURE;
    }
    printf("Shared memory opened\n");

    ftruncate(shm_fd, SIZE);
    printf("Shared memory size set to %d bytes\n", SIZE);

    int matrix1[m][n], matrix2[n][k];
    pid_t processes[n];
    srand(time(NULL)); // For random numbers

    // Generating matrices
    printf("Generating matrix1:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix1[i][j] = rand() % 10 + 1;
            printf("matrix1[%d][%d] = %d\n", i, j, matrix1[i][j]);
        }
    }

    printf("\nGenerating matrix2:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            matrix2[i][j] = rand() % 10 + 1;
            printf("matrix2[%d][%d] = %d\n", i, j, matrix2[i][j]);
        }
    }

    // Mapping shared memory
    ptr = (int (*)[k])mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("Shared memory mapped\n");

    // Forking processes
    printf("Starting matrix multiplication\n");
    for (int i = 0; i < m; i++) {
        processes[i] = fork();
        if (processes[i] == 0) { // Child process
            printf("Child process %d started\n", i);
            for (int j = 0; j < k; j++) {
                int sum = 0;
                for (int p = 0; p < n; p++) {
                    sum += matrix1[i][p] * matrix2[p][j];
                }
                ptr[i][j] = sum;
                printf("Child process %d: ptr[%d][%d] = %d\n", i, i, j, sum);
            }
            exit(0);
        }
    }

    // Waiting for child processes
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    printf("Matrix multiplication completed\n");

    // Printing the result matrix
    printf("Result matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            printf("%d ", ptr[i][j]);
        }
        printf("\n");
    }

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(NAME);
    printf("Resources cleaned up\n");

    return EXIT_SUCCESS;
}
