#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// #define NUM_THREADS	4

//if givern argument is prime returns the argument, if not returns 0
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Argument needed (amount of first primes to sum)\n");
        exit(0);
    }

    const int to_check = atoi(argv[1]);
    
    if (to_check < 1 )
    {
        fprintf(stderr, "positive integer is required\n");
        exit(EXIT_FAILURE);
    }

    long sum = 0;
    int prime = 0;
    int prime_count = 0;

    for (int i=2; prime_count < to_check; i++)
    {
        if (prime = is_prime(i))
        {
            // printf("%d\n", prime);
            prime_count++;
            sum += prime;
        }
    }

    printf("The sum of %d first primes is %li\n", to_check, sum);

    exit(EXIT_SUCCESS);
}