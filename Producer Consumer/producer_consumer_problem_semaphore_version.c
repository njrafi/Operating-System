#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>


// Queue Implementation where MAX is the buffer size

#define MAX_BUFFER 10

int buffer[MAX_BUFFER];
int fill_ptr = 0;
int use_ptr = 0;
int elementCount = 0;

void put(int value) {
        buffer[fill_ptr] = value;
        fill_ptr++;
        fill_ptr %= MAX_BUFFER;
        elementCount++;
         printf("Putting to queue: %d\n", value);
}

int getFront() {
        int value = buffer[use_ptr];
        use_ptr++;
        use_ptr %= MAX_BUFFER;
        elementCount--;
         printf("Performing task: %d\n", value);
        return value;
}

// queue implementation end

sem_t mutex,elementsInQueue,emptySpacesInQueue;

int currentTask = 0;
int maxTasks = 100;

void *produce(void *arg) {
        while(1) {

                sem_wait(&emptySpacesInQueue);
                sem_wait(&mutex);


                if(currentTask != maxTasks) {
                        printf("Producer %d: ", *(int*)arg + 1);
                        put(currentTask);
                        currentTask++;
                } else {
                        sem_post(&mutex);
                        break;
                }

                sem_post(&mutex);
                sem_post(&elementsInQueue);
        }

        printf("Producer %d: Produce finished\n", *(int*)arg + 1);
        return NULL;
}

void *consume(void *arg) {
        while(1) {

                sem_wait(&elementsInQueue);
                sem_wait(&mutex);

                if(elementCount > 0) {
                        printf("Consumer %d: ", *(int*)arg + 1);
                        int taskDone = getFront();
                }

                if(elementCount == 0 && currentTask == maxTasks) {
                        sem_post(&mutex);
                        sem_post(&elementsInQueue);
                        break;
                }

                sem_post(&mutex);
                sem_post(&emptySpacesInQueue);
        }
        printf("Consumer %d: consume finished\n", *(int*)arg + 1);
        return NULL;
}

#define MAX_PRODUCER 4
#define MAX_CONSUMER 4

int main() {

        sem_init(&mutex, 0, 1);
        sem_init(&elementsInQueue, 0, 0);
        sem_init(&emptySpacesInQueue, 0, MAX_BUFFER);

        pthread_t consumers[MAX_CONSUMER], producers[MAX_PRODUCER];
        int threadNumbers[MAX_CONSUMER + MAX_PRODUCER];

        for(int i = 0 ; i < MAX_CONSUMER; i++){
                threadNumbers[i] = i;
                pthread_create(&consumers[i], NULL, consume, &threadNumbers[i]);
        }

        for(int i = 0 ; i < MAX_PRODUCER; i++) {
                threadNumbers[MAX_CONSUMER + i] = i;
                pthread_create(&producers[i], NULL, produce, &threadNumbers[MAX_CONSUMER + i]);
        }


        for(int i = 0 ; i < MAX_CONSUMER; i++)
                pthread_join(consumers[i], NULL);

        for(int i = 0 ; i < MAX_PRODUCER; i++)
                pthread_join(producers[i], NULL);

        printf("Done %d \n", currentTask);
        return 0;
}
