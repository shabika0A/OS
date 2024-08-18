#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 10
#define NUMBER_OF_RESOURCES 5

typedef struct {
    int resources[NUMBER_OF_RESOURCES];
    sem_t availableResources;
    pthread_mutex_t poolMutex;
} ResourceManager;

ResourceManager resourceManager;

void* p(void* arg) {
    int threadId = *((int*)arg);
    int taskNumber = *((int*)(arg + sizeof(int)));
    printf("Thread %d has %d tasks.\n",threadId,taskNumber);

    for (int i = 0; i < taskNumber; i++) {
        int resourceId;
        sem_wait(&resourceManager.availableResources);
        pthread_mutex_lock(&resourceManager.poolMutex);

        // Find a free resource
        for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
            if (resourceManager.resources[i] == -1) {
                resourceId = i;
                resourceManager.resources[i] = threadId;
                break;
            }
        }

        printf("Thread %d is performing work with resource %d for task %d\n", threadId, resourceId+1, i+1);

        pthread_mutex_unlock(&resourceManager.poolMutex);
        int sleepTime = rand() % 5 + 1;  
        sleep(sleepTime);

        pthread_mutex_lock(&resourceManager.poolMutex);
        resourceManager.resources[resourceId] = -1;
        sem_post(&resourceManager.availableResources);
        pthread_mutex_unlock(&resourceManager.poolMutex);
    }

    return NULL;
}

int main() {
    sem_init(&resourceManager.availableResources, 0, NUMBER_OF_RESOURCES);
    pthread_mutex_init(&resourceManager.poolMutex, NULL);

    pthread_t threads[NUMBER_OF_THREADS];
    int threadIds[NUMBER_OF_THREADS];
    int taskNumbers[NUMBER_OF_THREADS];
    srand(time(NULL));

    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        resourceManager.resources[i] = -1;
    }

    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        threadIds[i] = i + 1;  // Thread IDs start from 1
        taskNumbers[i] = rand() % 4 + 1;  // Random task number between 1 and 4

        int* args = malloc(sizeof(int) * 2);
        args[0] = threadIds[i];
        args[1] = taskNumbers[i];

        pthread_create(&threads[i], NULL, p, args);
    }

    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&resourceManager.availableResources);
    pthread_mutex_destroy(&resourceManager.poolMutex);

    return 0;
}
