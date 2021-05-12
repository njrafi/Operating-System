#include <stdio.h>
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

// consumer threads waits on the condition empty and signal filled
// Producer threads waits on the condition filled and signal empty
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t filled = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int currentTask = 0;
int maxTasks = 100;

void *produce(void *arg) {
        while(1) {
                pthread_mutex_lock(&mutex);

                if(currentTask == maxTasks && elementCount == 0) {
                        pthread_cond_broadcast(&filled);
                        pthread_cond_broadcast(&empty);
                        pthread_mutex_unlock(&mutex);
                        break;
                }

                while(elementCount == MAX_BUFFER)
                        pthread_cond_wait(&filled,&mutex);

                if(currentTask != maxTasks) {
                        printf("Producer %d: ", *(int*)arg + 1);
                        put(currentTask);
                        currentTask++;
                }

                pthread_cond_signal(&empty);
                pthread_mutex_unlock(&mutex);
        }

        printf("Producer %d: Produce finished\n", *(int*)arg + 1);
        return NULL;
}

void *consume(void *arg) {
        while(1) {
                pthread_mutex_lock(&mutex);

                while(elementCount == 0 && currentTask < maxTasks)
                        pthread_cond_wait(&empty,&mutex);

                if(elementCount == 0 && currentTask == maxTasks) {
                        pthread_cond_broadcast(&filled);
                        pthread_cond_broadcast(&empty);
                        pthread_mutex_unlock(&mutex);
                        break;
                }

                printf("Consumer %d: ", *(int*)arg + 1);
                int taskDone = getFront();
                pthread_cond_signal(&filled);

                pthread_mutex_unlock(&mutex);
        }
        printf("Consumer %d: consume finished\n", *(int*)arg + 1);
        return NULL;
}

#define MAX_PRODUCER 3
#define MAX_CONSUMER 3

int main() {
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
