#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PACKET_SIZE 200
#define FIFOS_PATH "/home/wmularcz/Dokumenty/SCR_so/LAB_6/FIFOs/"

int main(int argc, char *argv[]){

	int potok_fd[2], licz, status;
	char bufor[PACKET_SIZE];

	FILE *fptr;

	for (int i=1; i < argc; i++)
	{
		fptr = fopen(argv[i],"r");

		char name[32];
		snprintf(name, sizeof(char) * 32, "fifo%i", i);

		int size = strlen(FIFOS_PATH) + strlen(name) + 1;
		char *file_path = (char *)malloc(size);
		strcpy(file_path, FIFOS_PATH);
		strcat(file_path, name);


		mkfifo(file_path, 0666);
		int fd = open(file_path, O_WRONLY);

		printf("przed pętlą");

		while (licz=fread(bufor, 1, PACKET_SIZE, fptr))
		{
			write(fd, bufor, licz);
		}

		fclose(fptr);
		close(fd);

		free(file_path);
	}

	return 0;
}
