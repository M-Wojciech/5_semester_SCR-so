#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>


pthread_mutex_t muteks_pobierania = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t muteks_sumowania = PTHREAD_MUTEX_INITIALIZER;

int N; /* liczba liczb pierwszych do znalezienia (arg.wywolania) */
int K; /* liczba watkow obliczeniowych (drugi arg.wywolania) */
int sprawdz = 2; /* nastepna liczba do sprawdzania */
int znalezione = 0; /* liczba znalezionych i zsumowanych l.pierwszych */
long suma = 0; /* to bedzie obliczona suma liczb pierwszych */
int *last_chceked_in_thread_ptr;

int compare (const void *num1, const void *num2)
{
   return ( *(int *)num2 - *(int *)num1 );
}


//if given argument is prime returns the argument, if not returns 0
int is_prime (int number)
{
    for (int i = 2; i < number; i++)
    {
        if((float)(number % i) == 0)
        {
            return 0;
        }
    }

    return number;
}

void *thread_prime_count(void *id)
{
    int current_to_check;

    while(1)
    {
        pthread_mutex_lock(&muteks_pobierania);
        current_to_check = sprawdz++;
        pthread_mutex_unlock(&muteks_pobierania);

        if (is_prime(current_to_check))
        {
            pthread_mutex_lock(&muteks_sumowania);
            znalezione++;
            suma += current_to_check;
            pthread_mutex_unlock(&muteks_sumowania);
            last_chceked_in_thread_ptr[last_chceked_in_thread_ptr[K*K+(int)(__intptr_t)id]*K+(int)(__intptr_t)id] = current_to_check;  
            last_chceked_in_thread_ptr[K*K+(int)(__intptr_t)id] = (last_chceked_in_thread_ptr[K*K+(int)(__intptr_t)id]+1)%K;
        }

        if (znalezione >= N)
        {
            break;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Arguments needed (amount of first primes to sum and number of threads)\n");
        exit(0);
    }

    N = atoi(argv[1]);
    K = atoi(argv[2]);
    
    if (N < 1 )
    {
        fprintf(stderr, "positive integer is required as number of primes to be summed\n");
        exit(EXIT_FAILURE);
    }

    if (K < 1 )
    {
        fprintf(stderr, "positive integer is required as a number of threads\n");
        exit(EXIT_FAILURE);
    }

    int last_chceked_in_thread_arr[(K+1)*K];
    for (int i; i<K*(K+1); i++)
    {
        last_chceked_in_thread_arr[i] = 0;
    }
    last_chceked_in_thread_ptr = last_chceked_in_thread_arr;

    pthread_t thread[K];
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_init(&attr);

    for(int i=0; i<K; i++)
    {
        pthread_create(&thread[i], &attr, thread_prime_count, (void *)(intptr_t)i);
    }

    for(int i=0; i<K; i++)
    {
        pthread_join(thread[i], NULL);
    }

    int overdone = znalezione-N;

    if ( overdone > 0)
    {
        qsort(last_chceked_in_thread_arr, K*K, sizeof(*last_chceked_in_thread_arr), compare);

        for (int i=0; i < overdone; i++)
        {
            suma -= last_chceked_in_thread_arr[i];
        }
    }


    printf("The sum of %d first primes is %li\n", N, suma);

    exit(EXIT_SUCCESS);
}
