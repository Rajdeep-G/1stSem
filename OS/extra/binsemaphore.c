#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2

int sharedResource = 0;
int binarySemaphore = 1; // 1 represents an unlocked semaphore

void *increment(void *arg)
{
    int i;
    for (i = 0; i < 100000; i++)
    {
        // Acquire the semaphore (wait for it to be unlocked)
        while (1)
        {
            if (binarySemaphore == 1)
            {
                binarySemaphore = 0; // Lock the semaphore
                break;
            }
        }
        sharedResource++;
        binarySemaphore = 1; // Release the semaphore
    }
    return NULL;
}

void *decrement(void *arg)
{
    int i;
    for (i = 0; i < 100000; i++)
    {
        // Acquire the semaphore (wait for it to be unlocked)
        while (1)
        {
            if (binarySemaphore == 1)
            {
                binarySemaphore = 0; // Lock the semaphore
                break;
            }
        }
        sharedResource--;
        binarySemaphore = 1; // Release the semaphore
    }
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;

    // Create two threads
    pthread_create(&threads[0], NULL, increment, NULL);
    pthread_create(&threads[1], NULL, decrement, NULL);

    // Wait for the threads to finish
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the final value of the shared resource
    printf("Shared Resource: %d\n", sharedResource);

    return 0;
}
