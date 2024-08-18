#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 2

char inputFileAddress[100];
char outFileAddress[100];

FILE *fp;
char *line = NULL;
size_t len = 0;
size_t r;

char buffer[BUFFER_SIZE][1000];
int count = 0;
int in = 0;
int out = 0;


pthread_mutex_t lock;
pthread_cond_t buffer_not_full;
pthread_cond_t buffer_not_empty;

int eof_flag = 0; // Flag to indicate end of file

void *producer(void *arg);
void *consumer(void *arg);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(inputFileAddress, argv[1]);
    strcpy(outFileAddress, argv[2]);

    pthread_cond_init(&buffer_not_full, NULL);
    pthread_cond_init(&buffer_not_empty, NULL);

    pthread_mutex_init(&lock, NULL);

    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&buffer_not_full);
    pthread_cond_destroy(&buffer_not_empty);

    if (line) {
        free(line);
    }

    exit(EXIT_SUCCESS);
}

void *producer(void *arg) {
    fp = fopen(inputFileAddress, "r");
    if (fp == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    while ((r = getline(&line, &len, fp)) != -1) {
        pthread_mutex_lock(&lock);

        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&buffer_not_full, &lock);
            printf("......producer is waiting...\n");
        }

        // Add line to the buffer
        strcpy(buffer[in], line);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&buffer_not_empty);

    }

    fclose(fp);
    pthread_mutex_lock(&lock);
    eof_flag = 1;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&buffer_not_empty);
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    FILE *outputFile;
    outputFile = fopen(outFileAddress, "a");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    while (1 && !eof_flag) {
        pthread_mutex_lock(&lock);

        // Wait if the buffer is empty and the end of file is not reached
        while (count == 0 && !eof_flag) {
            pthread_cond_wait(&buffer_not_empty, &lock);
            printf("......consumer is waiting...\n");
        }

        // Check if the buffer is empty and the end of file is reached
        if (count == 0 && eof_flag) {
            pthread_mutex_unlock(&lock);
            break; // Exit the loop if both conditions are met
        }

        printf("Consumed line: %s \n", buffer[out]);
        fprintf(outputFile, "%s", buffer[out]);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&buffer_not_full);
    }

    pthread_exit(NULL);
}







