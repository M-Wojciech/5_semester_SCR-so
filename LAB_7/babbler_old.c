#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

#define BABBLE_NAME "/elita"
#define BABBLE_MODE 0777
#define BABBLE_LIMIT 32
#define BABBLE_LENGTH 80

typedef struct {
  pthread_mutex_t babble_mutex;
  pthread_cond_t babble_cond;
  int babble_first, babble_total;
  char babbles[BABBLE_LIMIT][BABBLE_LENGTH];
} babblespace;

int main (){
	
	babblespace *my_babblespace = (babblespace *)malloc(sizeof(babblespace));
	
	int fd_babble = shm_open(BABBLE_NAME, O_RDWR, BABBLE_MODE);

  my_babblespace = (babblespace *)mmap(NULL, sizeof(babblespace), PROT_READ|PROT_WRITE, MAP_SHARED, fd_babble, 0);
  

  pthread_mutex_lock(&my_babblespace->babble_mutex);

  printf("pierwszy babble: %d\n\n", my_babblespace->babble_first);

  // for( int i = my_babblespace->babble_first; i < my_babblespace->babble_first+BABBLE_LIMIT; i++)
  // {
  //   // printf("%d: %s\n",i-my_babblespace->babble_first+1 ,my_babblespace->babbles[i%BABBLE_LIMIT]);
  //   printf("%d: %s\n",i%BABBLE_LIMIT ,my_babblespace->babbles[i%BABBLE_LIMIT]);
  // }

  for( int i = 0; i < BABBLE_LIMIT; i++)
  {
    printf("%d: %s\n",(i+my_babblespace->babble_first)%32 ,my_babblespace->babbles[(my_babblespace->babble_first+i)%BABBLE_LIMIT]);
  }

  pthread_mutex_unlock(&my_babblespace->babble_mutex);


  char *buff = malloc(sizeof(char) * BABBLE_LENGTH);

  printf("\nWprowadź wiadomość:\n");

  fgets(buff, BABBLE_LENGTH, stdin);
  buff[strlen(buff)] = '\0';

  pthread_mutex_lock(&my_babblespace->babble_mutex);

  strcpy(my_babblespace->babbles[my_babblespace->babble_first+1], buff);

  pthread_mutex_unlock(&my_babblespace->babble_mutex);


  munmap((char *)my_babblespace, sizeof(babblespace));
      
  return 0;

}




