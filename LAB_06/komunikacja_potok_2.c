#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PACKET_SIZE 200

int main(int argc, char *argv[]){

    int potok_fd[2], licz, status;
    char bufor[PACKET_SIZE];

    pipe(potok_fd);

    if (fork() == 0) {

	//printf("child hello\n");

        close(potok_fd[1]); /* wazne */

       	dup2(potok_fd[0], STDIN_FILENO);

	execlp("display", "display", "-update", "1", NULL);

        //printf("going into loop child\n");
        
        //while (fread(bufor, 1, PACKET_SIZE, potok_fd[0])>0)
        //while ((licz=read(potok_fd[0], bufor, PACKET_SIZE)) > 0)
        //{
        //}
	//printf("child exit\n");
        exit(0);
    }


    FILE *fptr;
    fptr = fopen(argv[1],"rb");

    if(fptr == NULL)
    {
        printf("Error while opening the file!\n");   
        exit(1);             
    }

    while (licz=fread(bufor, 1, PACKET_SIZE, fptr))
    {
        write(potok_fd[1], bufor, licz);
    }
    close(potok_fd[1]);
    //printf("sent\n");
    wait(&status);
    //printf("parent closing\n");

    return(status);
}
