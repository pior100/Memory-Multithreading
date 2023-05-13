#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struktura przechowująca dane wątku
typedef struct {
    int threadId;
    int* numbers;
    int length;
} ThreadData;

// Funkcja wykonywana przez wątek
void* threadFunction(void* data) {
    ThreadData* threadData = (ThreadData*)data;

    // Sumowanie liczb w tablicy
    int sum = 0;
    for (int i = 0; i < threadData->length; ++i) {
        sum += threadData->numbers[i];
    }

    printf("Thread %d sum: %d\n", threadData->threadId, sum);

    // Zwolnienie pamięci zaalokowanej dla tablicy
    free(threadData->numbers);
    free(threadData);

    return NULL;
}

int main() {
    const int numThreads = 4;
    const int numNumbers = 1000000;
    const int numbersPerThread = numNumbers / numThreads;

    // Inicjalizacja tablicy liczb
    int* numbers = malloc(sizeof(int) * numNumbers);
    for (int i = 0; i < numNumbers; ++i) {
        numbers[i] = i + 1;
    }

    // Inicjalizacja wątków
    pthread_t threads[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        // Tworzenie danych dla wątku
        ThreadData* threadData = malloc(sizeof(ThreadData));
        threadData->threadId = i;
        threadData->length = numbersPerThread;

        // Tworzenie podtablicy dla wątku
        threadData->numbers = malloc(sizeof(int) * numbersPerThread);
        for (int j = 0; j < numbersPerThread; ++j) {
            threadData->numbers[j] = numbers[i * numbersPerThread + j];
        }

        // Tworzenie wątku
        pthread_create(&threads[i], NULL, threadFunction, threadData);
    }

    // Oczekiwanie na zakończenie wątków
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Zwolnienie pamięci zaalokowanej dla tablicy
    free(numbers);

    return 0;
}
