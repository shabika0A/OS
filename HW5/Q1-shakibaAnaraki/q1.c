#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <string.h>

int mypipe[3][2];
pthread_mutex_t lock;
int numCountWasSet = 0;

FILE *fp;
int in = 0, out = 0, count = 0, numCount = 0;


int main()
{
    srand(time(NULL));

    pipe(mypipe[0]);
    pipe(mypipe[1]);
    pipe(mypipe[2]);
    pthread_mutex_init(&lock, NULL);

    fp = fopen("input2.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t r;

    if ((r = getline(&line, &len, fp)) == -1)
    {
        perror("Error reading from input file");
        exit(EXIT_FAILURE);
    }

    numCount = atoi(line);

    int pid1 = fork();
    if (pid1 > 0)
    {
        int temp;
        for (int i = 0; i < numCount; i++)
        {
            int num;
            r = getline(&line, &len, fp);
            num = atoi(line);
            printf("input: %d\n", num);
            write(mypipe[1][1], &num, sizeof(int));
            write(mypipe[2][1], &num, sizeof(int));  
            // printf("write %d\n", num);
        }

        fclose(fp);

        float min_result,avg_result;
        read(mypipe[0][0], &min_result, sizeof(float));
        read(mypipe[0][0], &avg_result, sizeof(float));

        printf("Mean = %.2f, Min = %.2f\n", avg_result, min_result);

        exit(0);
    }

    int pid2 = fork();
    if (pid2 > 0)
    {
        float min = 100000000;
        int temp;
        // printf ("min: %f\n",min);

        int i = 0;

        while (i < numCount)
        {
            read(mypipe[1][0], &temp, sizeof(int));
            // printf("output -> %d\n", temp);

            printf("read1 <- %d\n", temp);
            i++;

            if (min > temp)
            {
                min = temp;
            }
            // printf ("min: %d\n",min);
        }
        write(mypipe[0][1], &min, sizeof(int));
        exit(0);
    }

    int pid3 = fork();
    if (pid3 > 0)
    {
        int sum = 0;
        int temp;

        for (int i = 0; i < numCount; i++)
        {
            read(mypipe[2][0], &temp, sizeof(int));
            // printf("output -> %d\n", temp);
            sum += temp;
            printf("read2 <- %d\n", temp);
        }
        float average = (float)sum / numCount;


        write(mypipe[0][1], &average, sizeof(float));

        exit(0);
    }

    wait(NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}
