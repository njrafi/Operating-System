#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
        int x,y;
} Point;

typedef struct {
        int x,y,c;
} Result;

void* multiplyByTwo(void* arg) {
        Point* point = (Point*)arg;
        printf("point: %d %d\n", point->x,point->y);

        Result* result = malloc(sizeof(Result));
        result->x = point->x * 2;
        result->y = point->y * 2;
        result->c = 13;

        return result;
}

int main() {

        pthread_t thread1,thread2;
        Point point1 = {10,20};
        Point point2 = {15,25};

        pthread_create(&thread1, NULL, multiplyByTwo, &point1);
        pthread_create(&thread2, NULL, multiplyByTwo, &point2);

        Result* result1;
        Result* result2;

        pthread_join(thread1, &result1);
        pthread_join(thread2, &result2);

        printf("Result1: %d %d %d\n", result1->x, result1->y, result1->c);
        printf("Result2: %d %d %d\n", result2->x, result2->y, result2->c);

        return 0;
}
