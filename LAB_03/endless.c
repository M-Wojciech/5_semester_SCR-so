#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define ASCII_ESC 27

void sighandler(int signum)
{
    switch (signum)
    {
        case SIGALRM:
            printf("SIGALRM\n");
            break;
        case SIGTERM:
            printf("SIGTERM\n");
            exit(1);
        case SIGUSR1:
            printf("SIGUSR1\n");
            break;
        case SIGUSR2:
            printf("SIGUSR2\n");
            break;
    }
}

int main()
{
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sighandler);
    signal(SIGUSR1, sighandler);
    signal(SIGUSR2, sighandler);

    sigset_t block_usr2;

    sigemptyset (&block_usr2);
    sigaddset (&block_usr2, SIGUSR2);

    int i, j=1;

    while(1)
    {
        i=0;

        sigprocmask (SIG_BLOCK, &block_usr2, NULL);

        while(i < 500)
        {  
            i++;
            nanosleep((const struct timespec[]){{0, 10000000L}}, NULL);
        }

	printf("%c[1A", ASCII_ESC );
   	printf( "%c[2K", ASCII_ESC );
        printf("Odblokowuje po raz %d\n",j);
        sigprocmask (SIG_UNBLOCK, &block_usr2, NULL);
	
	j++;
    }
}
