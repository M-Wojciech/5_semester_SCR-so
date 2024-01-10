#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h> 

#define NUM_THREADS	4

pthread_barrier_t barrier;

static void *thread_func(void *id)
{
    printf("W wątku%d\n", (__intptr_t)id);
    pthread_barrier_wait(&barrier);
    printf("za barierą wątku%d\n", (__intptr_t)id);

    int wynik;
    for (int i=0; i<10; ++i) wynik += log(i);

    printf("Hello OpSys. Written by thread %d\n", (intptr_t)id);

    pthread_exit(NULL);
}

int main()
{
    pthread_barrier_init(&barrier, NULL, NUM_THREADS+1);

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_attr_init(&attr);

    long pid = getpid();

    for(int i=0; i<NUM_THREADS; i++)
    {
        pthread_create(&thread[i], &attr, &thread_func, (void *)(intptr_t) i);
    }

    pthread_barrier_wait(&barrier);
    pthread_barrier_destroy(&barrier);

    pthread_exit(NULL);
}