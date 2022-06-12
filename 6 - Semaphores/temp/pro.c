#include<stdio.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include<time.h>

#define BUFSIZE 10

struct memory {
  int buffer[BUFSIZE];
  int count;
  sem_t full;
  sem_t empty;
  sem_t mutex;
  int n;
  int nstatus;
};
struct memory * shmptr;
int main() {
  srand(time(0));
  int shmid = shmget(111, sizeof(struct memory), IPC_CREAT | 0666);
  shmptr = (struct memory * ) shmat(shmid, NULL, 0);
  shmptr -> nstatus = 0;
  shmptr -> count = 0;
  printf("\nProducer and Consumer Problem\n");
  // printf("Number of terms : ");
  //scanf("%d",&(shmptr->n));
  while (1) {
    if (shmptr -> nstatus != 0) {
      int i = shmptr -> n;
      do {
        sem_wait( & (shmptr -> empty));
        sem_wait( & (shmptr -> mutex));
        shmptr -> buffer[shmptr -> count++] = rand() % 100;
        printf("Produced %d\n", shmptr -> buffer[shmptr -> count - 1]);
        i--;
        sem_post( & (shmptr -> mutex));
        sem_post( & (shmptr -> full));
        sleep(1);
      } while (i > 0);
      printf("\nproducer complete\n");
      if (i == 0) break;
    }
  }
  shmdt(shmptr);
  shmctl(shmid, IPC_RMID, NULL);
  sem_destroy( & (shmptr -> empty));
  sem_destroy( & (shmptr -> full));
  sem_destroy( & (shmptr -> mutex));
  printf("Done\n");
  exit(1);
}