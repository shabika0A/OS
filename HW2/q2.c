#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main()
{
    char filename[100];
    char start[10];
    char words[10][80];
    char line[100];

    scanf("%s", start);
    scanf("%99s", filename);

    int wordCount = 0;
    while (1)
    {
        scanf("%99s", words[wordCount]);
        if (strcmp(words[wordCount], ".") == 0)
        {
            break; // Exit the loop when '.' is entered
        }
        wordCount++;
    }

    int totalOccurrences = 0;

    for (int i = 0; i < wordCount; i++)
    {
        pid_t child_pid = vfork();

        // if (child_pid == -1) {
        //     perror("Error forking a child process");
        //     exit(1);
        // }

        if (child_pid == 0)
        { // Child process
            FILE *file = fopen(filename, "r");

            // if (file == NULL) {
            //     perror("Error opening the file");
            //     exit(1);
            // }

            int occurrences = 0;

            while (fgets(line, sizeof(line), file))
            {
                if (strstr(line, words[i]) != NULL)
                {
                    occurrences++;
                    totalOccurrences++;
                }
            }

            printf("query%d: %d\n", i + 1, occurrences);

            fclose(file);
            exit(0);
        }
        else
        { // Parent process
            int status;
            wait(&status);
        }
    }

    printf("Total occurrences: %d\n", totalOccurrences);

    return 0;
}
