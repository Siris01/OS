#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
  
void send(char* name) {
    key_t key = ftok("shmfile", 65);  
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
  
    char *str = (char*) shmat(shmid, (void*)0, 0);
    strcpy(str, name);
    printf("[P1] Sent name: %s\n", str);

    shmdt(str);
}

void recieve() {
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
  
    char *str = (char*) shmat(shmid, (void*)0, 0);

    printf("[P2] Recieved name: %s\n", str);

    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i]>='a' && str[i]<='z') str[i] = str[i] - 32;
    }

    printf("[P2] Uppercased name: %s\n", str);
   
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
}

int main() {
    char* name = (char*) malloc(sizeof(char) * 100);
    printf("[MAIN] Enter name: ");
    scanf("%s", name);

    int P1 = fork();

    if (P1 == 0) {
        send(name);

        int P2 = fork();
        if (P2 == 0) recieve();   
    }
    
    return 0;
}
