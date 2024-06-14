#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define BABBLE_NAME "/elita"
#define BABBLE_MODE 0777
#define BABBLE_LIMIT 32
#define BABBLE_LENGTH 80

typedef struct
{
  pthread_mutex_t babble_mutex;
  pthread_cond_t babble_cond;
  int babble_first, babble_total;
  char babbles[BABBLE_LIMIT][BABBLE_LENGTH];
} babblespace;

int terminate = 0, new_mess = 1;

void sig_handle()
{
  terminate = 1;
}
void sig_handle_2()
{
  new_mess = !new_mess;
}

void clear_input()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ask_for_babble_reset()
{
  char answer;

  while(1)
  {
    printf("Do you want to clear all babbles? (Y/N)\n");

    scanf(" %c", &answer);
    clear_input();

    switch (answer)
    {
      case 'y':
      case 'Y':
        return 1;
      case 'n':
      case 'N':
        return 0;
      default:
        printf("\033[1;1H\033[2J");
        printf("Incorrect input.\n");
        continue;
    }
  }
}

void print_babbles(babblespace *my_babblespace)
{
  printf("\033[1;1H");
  
  for( int i = 0; i < my_babblespace->babble_total; i++)
  {
    printf("\033[2K");
    printf("%d: %s\n",(i+my_babblespace->babble_first)%32 ,my_babblespace->babbles[(my_babblespace->babble_first+i)%BABBLE_LIMIT]);
  }
}

void print_babbles_mutex(babblespace *my_babblespace)
{
  pthread_mutex_lock(&my_babblespace->babble_mutex);

  print_babbles(my_babblespace);

  pthread_mutex_unlock(&my_babblespace->babble_mutex);
}

void print_babbles_cond(babblespace *my_babblespace)
{
  int cond_check;
  struct timespec time;
  
  pthread_mutex_lock(&my_babblespace->babble_mutex);

  cond_check = my_babblespace->babble_first+my_babblespace->babble_total;
  while (cond_check == my_babblespace->babble_first+my_babblespace->babble_total)
  {
    if (terminate)
    {
      pthread_mutex_unlock(&my_babblespace->babble_mutex);
      exit(0);
    }
    clock_gettime(CLOCK_REALTIME, &time);
    if ( time.tv_nsec < 900*1000*1000)
    {
      time.tv_nsec += 100*1000*1000;
    } else 
    {
      time.tv_sec += 1;
      time.tv_nsec -= 900*1000*1000;
    }
    pthread_cond_timedwait(&my_babblespace->babble_cond, &my_babblespace->babble_mutex, &time);
  }

  print_babbles(my_babblespace);

  pthread_mutex_unlock(&my_babblespace->babble_mutex);
}

void send_babbles(babblespace *my_babblespace)
{
  char *buff = malloc(sizeof(char) * BABBLE_LENGTH);

  fgets(buff, BABBLE_LENGTH, stdin);
  printf("\033[A\033[2K");

  if (buff[strlen(buff)-1] != '\n')
  {
    clear_input();
  }
  buff[strlen(buff)-1] = '\0';

  pthread_mutex_lock(&my_babblespace->babble_mutex);

  strcpy(my_babblespace->babbles[(my_babblespace->babble_total + my_babblespace->babble_first)%BABBLE_LIMIT], buff);

  my_babblespace->babble_total==BABBLE_LIMIT?my_babblespace->babble_first++:my_babblespace->babble_total++;

  if (pthread_cond_broadcast(&my_babblespace->babble_cond) != 0) {
    perror("Error signaling condition variable: %d\n");
    exit(EXIT_FAILURE);
  }

  pthread_mutex_unlock(&my_babblespace->babble_mutex);

  free(buff);

}


int main (){
	
	babblespace *my_babblespace = (babblespace *)malloc(sizeof(babblespace));
  if (my_babblespace == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  printf("\033[1;1H\033[2J");
  int initialize_object = ask_for_babble_reset();

  if (initialize_object){
    if (shm_unlink(BABBLE_NAME)<0) {
      perror("shm_unlink failure");
      exit(EXIT_FAILURE);
    }
  }

	int fd_babble = shm_open(BABBLE_NAME, O_RDWR|O_CREAT, BABBLE_MODE);

  if (fd_babble<0) {
    perror("shm_open failure");
    exit(EXIT_FAILURE);
  }

  if (ftruncate(fd_babble, sizeof(babblespace))<0) {
    perror("ftruncate failure");
    exit(EXIT_FAILURE);
  }

  my_babblespace = (babblespace *)mmap(NULL, sizeof(babblespace), PROT_READ|PROT_WRITE, MAP_SHARED, fd_babble, 0);

  if (my_babblespace == MAP_FAILED) {
      perror("mapping file into memory failure");
      return 1;
  }

  if(initialize_object){
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    if (pthread_mutex_init(&my_babblespace->babble_mutex, &attr) != 0) {
      perror("Mutex initialization failed:");
      exit(EXIT_FAILURE);
    }

    pthread_condattr_t attr2;
    pthread_condattr_init(&attr2);
    pthread_condattr_setpshared(&attr2, PTHREAD_PROCESS_SHARED);
    if (pthread_cond_init(&my_babblespace->babble_cond, &attr2) != 0) {
      perror("Condition variable initialization failed:");
      exit(EXIT_FAILURE);
    }
  }

  printf("\033[1;1H\033[2J"); // set cursor to [1,1], clear screen

  pid_t pid = fork();
  if (pid == -1)
  {
    perror("fork failiure");
    exit(EXIT_FAILURE);
  } else if ( pid ==0)
  {
    signal(SIGINT, sig_handle);
    signal(SIGUSR1, sig_handle_2);
    print_babbles_mutex(my_babblespace);
    printf("\033[34;1H");
    printf("Enter message:\n");

    while(1)
    {
      printf("\0337");
      print_babbles_cond(my_babblespace);

      if (new_mess == 0)
      {
        printf("\033[34;1H\033[2K");
        printf("Do you want to send another message (Y/N)\n");
        printf("\0338");
        fflush(stdout);      
      } else
      {
        printf("\033[34;1H\033[2K");
        printf("Enter message:\n");
        printf("\0338");
        fflush(stdout);
      }
    }

    return 0;
  }

  char answer;
  int menu = 0;

  while(1)
  {
    if(menu==0)
    {
      send_babbles(my_babblespace);
      kill(pid, SIGUSR1);
    }

    scanf(" %c", &answer);
    printf("\033[1A\033[2K");
    clear_input();

    switch (answer)
    {
      case 'y':
      case 'Y':
        printf("\033[34;1H\033[2K");
        printf("Enter message:\n\033[0J");
        kill(pid, SIGUSR1);
        menu = 0;
        continue;
      case 'n':
      case 'N':
        break;
      default:
        printf("\033[37;1H");
        printf("Incorrect input.\n");
        printf("\033[35;1H");
        menu = 1;
        continue;
    }
    break;
  }

  if (kill(pid, SIGINT) == -1) {
    perror("Error killing child process");
    exit(EXIT_FAILURE);
  }

  munmap((char *)my_babblespace, sizeof(babblespace));
  
  printf("\033[1;1H\033[2J");
  
  return 0;
}

