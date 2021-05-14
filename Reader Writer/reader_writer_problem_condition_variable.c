#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define maxReader 6
#define maxWriter 3

int data = -1;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readPhase = PTHREAD_COND_INITIALIZER;
pthread_cond_t writePhase = PTHREAD_COND_INITIALIZER;

int numOfReader = 0;
int numOfWriter = 0;

void *readData(void *param);
void *writeData(void *param);

int max(int num1,int num2) {
    if(num1 > num2) return num1;
    return num2;
}

int main() {
    srand(time(NULL));
    pthread_t readers[maxReader],writers[maxWriter];
    int tem[max(maxReader,maxWriter)];

    for(int i = 0 ; i < maxWriter; i++) {
        tem[i] = i+1;
        pthread_create(&writers[i],NULL,writeData,&tem[i]);
    }

   for(int i = 0 ; i < maxReader; i++) {
        tem[i] = i+1;
        pthread_create(&readers[i],NULL,readData,&tem[i]);
   }

    for(int i = 0 ; i < maxWriter; i++)
        pthread_join(writers[i],NULL);


    for(int i = 0 ; i < maxReader; i++)
        pthread_join(readers[i],NULL);


   return 0;
}

void *readData(void *params) {
    int number = *((int*)params);
    for(int i = 0 ; i < 20 ; i++) {

        pthread_mutex_lock(&m);

        while(numOfWriter)
            pthread_cond_wait(&readPhase,&m);

        numOfReader++;

        pthread_mutex_unlock(&m);

        printf("Reader = %d, numOfReader = %d, numOfWriter = %d, Read =  %d\n", number,numOfReader,numOfWriter,data);


        pthread_mutex_lock(&m);

        numOfReader--;

        pthread_cond_signal(&writePhase);

        pthread_mutex_unlock(&m);


    }
}

void *writeData(void *params) {
    int number = *((int*)params);

    for(int i = 0 ; i < 20 ; i++) {

        pthread_mutex_lock(&m);

        while(numOfWriter || numOfReader)
            pthread_cond_wait(&writePhase,&m);

        numOfWriter++;

        pthread_mutex_unlock(&m);

        data = rand() % 100;
        printf("Writer = %d, numOfReader = %d, numOfWriter = %d, Write =  %d\n", number,numOfReader, numOfWriter, data);



        pthread_mutex_lock(&m);

        numOfWriter--;
        pthread_cond_broadcast(&readPhase);
        pthread_cond_signal(&writePhase);

        pthread_mutex_unlock(&m);
    }
}

