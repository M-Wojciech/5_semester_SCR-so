#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 10 //sons
#define S 20 //chances
#define X 10 //width
#define Y 20 //length

int land[X*Y];
pthread_mutex_t lower_priority_mutex;
pthread_mutex_t land_mutex;
pthread_cond_t done_condvar;
int done_count = 0;
// int done_id = 0;
struct timespec wait;

int queue_array[N];


void *son(void *id)
{
    int random;
    
    pthread_mutex_lock(&lower_priority_mutex);
    pthread_mutex_lock(&land_mutex);

    for ( int i = 0; i < S; i++)
    {
        pthread_mutex_unlock(&land_mutex);
        pthread_mutex_unlock(&lower_priority_mutex);
        random = ((int)(X*Y*drand48()))%(X*Y);
        pthread_mutex_lock(&lower_priority_mutex);
        pthread_mutex_lock(&land_mutex);
        if (land[random] == 0)
        {
            land[random] = (int)(long)id;
        }
    }
    // done_id = (int)(long)id;
    queue_array[(int)(long)id-1] = 1;
    done_count++;
    pthread_cond_signal(&done_condvar);
    pthread_mutex_unlock(&land_mutex);
    // nanosleep(&wait, NULL);
    pthread_mutex_unlock(&lower_priority_mutex);

    pthread_exit(NULL);
}

void *vivaldini(void *id)
{
    pthread_mutex_lock(&land_mutex);
    int free_real_estate = X*Y;
    int sons_land = 0;

    while (done_count < N)
    {
        pthread_cond_wait(&done_condvar, &land_mutex);

        for(int j=0; j < N; j++)
        {
            if (queue_array[j] == 1)
            {
                for (int i=0; i < X*Y; i++)
                {
                    if (land[i] == j+1)
                    sons_land++;
                }

                free_real_estate -= sons_land;
                printf("Syn %d zdobył %d terytoriów\n", j+1, sons_land);
                sons_land = 0;
            }
            queue_array[j] = 0;
        }
    }
    pthread_mutex_unlock(&land_mutex);

    printf("Po walkach pozostało %d wolnych terytoriów\n", free_real_estate);

    pthread_exit(NULL);
}

int main()
{   
    wait.tv_sec=0;
    wait.tv_nsec=10000;
    srand48((long)time(NULL));
    pthread_t threads[N+1];
    pthread_attr_t attr;

    pthread_mutex_init(&lower_priority_mutex, NULL);
    pthread_mutex_init(&land_mutex, NULL);
    pthread_cond_init (&done_condvar, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < X*Y; i++)
    {
        land[i] = 0;
    }

    for (int i = 0; i < N; i++)
    {
        queue_array[i] = 0;
    }

    pthread_create(&threads[0], &attr, vivaldini, (void *)0);

    for (long i = 1; i <= N; i++)
    {
        pthread_create(&threads[i], &attr, son, (void *)i);
    }


    for (long i = 0; i < N+1; i++)
    {
        pthread_join(threads[i], NULL);
    }
}