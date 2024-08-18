#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 10
#define MAX_INPUT_SIZE 100

void executeCommand(char **args, int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid == 0) {

        if (input_fd != 0) {
            dup2(input_fd, 0);
            close(input_fd);
        }

        if (output_fd != 1) {
            dup2(output_fd, 1);
            close(output_fd);
        }

        fprintf(stderr, "Executing command: ");
        for (int i = 0; args[i] != NULL; i++) {
            fprintf(stderr, "%s ", args[i]);
        }
        fprintf(stderr, "\n");

        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    waitpid(pid, NULL, 0);
}

void executePipeline(char ***commands, int numCommands) {
    int i;
    int input_fd = 0;  // Initial input is stdin

    int mypipe[numCommands - 1][2];

    for (i = 0; i < numCommands - 1; i++) {
        if (pipe(mypipe[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < numCommands; i++) {
        int output_fd = (i == numCommands - 1) ? 1 : mypipe[i][1];

        executeCommand(commands[i], input_fd, output_fd);

        if (i > 0) {
            close(mypipe[i - 1][1]);
        }

        if (i < numCommands - 1) {
            input_fd = mypipe[i][0];
        }
    }

    // Close the remaining open file descriptors
    for (i = 0; i < numCommands - 1; i++) {
        close(mypipe[i][0]);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Enter the command: ");
    fgets(input, sizeof(input), stdin);

    // Remove the newline character from the input
    input[strcspn(input, "\n")] = '\0';

    char *token;
    char *subtoken;
    char ***commands; 
    int numCommands = 0;
    char *saveptr1, *saveptr2;

    // First pass to count the number of commands
    for (token = strtok_r(input, "|", &saveptr1); token != NULL; token = strtok_r(NULL, "|", &saveptr1)) {
        numCommands++;
    }

    // Reset input string for the second pass
    memcpy(input, saveptr1, MAX_INPUT_SIZE);

    // Allocate memory for commands array
    commands = (char ***)malloc(numCommands * sizeof(char **));

    int i = 0;
    for (token = strtok_r(input, "|", &saveptr1); token != NULL; token = strtok_r(NULL, "|", &saveptr1)) {
        int argCount = 0;
        commands[i] = (char **)malloc(MAX_ARGS * sizeof(char *));

        // Split each command by spaces to get the arguments
        for (subtoken = strtok_r(token, " ", &saveptr2); subtoken != NULL; subtoken = strtok_r(NULL, " ", &saveptr2)) {
            commands[i][argCount++] = subtoken;
        }
        commands[i][argCount] = NULL; 
        i++;
    }

    executePipeline(commands, numCommands);

    // Free allocated memory
    for (int j = 0; j < numCommands; j++) {
        free(commands[j]);
    }
    free(commands);

    return 0;
}


