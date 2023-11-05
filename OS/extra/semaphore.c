#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 3

int sharedResource = 0;
sem_t countingSemaphore;

void *increment(void *arg)
{
    int i;
    for (i = 0; i < 100000; i++)
    {
        sem_wait(&countingSemaphore); // Decrement the semaphore
        sharedResource++;
        sem_post(&countingSemaphore); // Increment the semaphore
    }
    return NULL;
}

void *decrement(void *arg)
{
    int i;
    for (i = 0; i < 100000; i++)
    {
        sem_wait(&countingSemaphore); // Decrement the semaphore
        sharedResource--;
        sem_post(&countingSemaphore); // Increment the semaphore
    }
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;

    // Initialize the counting semaphore with an initial value of 2
    sem_init(&countingSemaphore, 0, 2);

    // Create two threads
    pthread_create(&threads[0], NULL, increment, NULL);
    pthread_create(&threads[1], NULL, decrement, NULL);

    // Create another thread to demonstrate the counting feature
    pthread_create(&threads[2], NULL, increment, NULL);

    // Wait for the threads to finish
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the final value of the shared resource
    printf("Shared Resource: %d\n", sharedResource);

    // Destroy the counting semaphore
    sem_destroy(&countingSemaphore); 
    sem_wait(&countingSemaphore);
    return 0;
}
