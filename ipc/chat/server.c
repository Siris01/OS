#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h> 

typedef struct packet {
    char user[25];
    char message[100];
    int notif;
} packet;
  
void listener() {

    packet *p = (packet*) shmat(shmget(ftok("shmfile", 65), sizeof(packet), 0666|IPC_CREAT), (void*)0, 0);

    while (1) {
        if (p->notif == 1) {
            printf("[%s]: %s\n", p->user, p->message);
            if (strcmp(p->user, "SYSTEM") == 0) {
                shmdt(p);
                exit(0);
            }
            p->notif = 0;
        }
    }
}

void sender(char *user) {

    packet *p = (packet*) shmat(shmget(ftok("shmfile", 65), sizeof(packet), 0666|IPC_CREAT), (void*)0, 0);

    while (1) {
        char *message = (char*) malloc(sizeof(char) * 100);
        scanf("%[^\n]%*c", message);

        if (strcmp(message, "\\exit") == 0) {
            printf("[SYSTEM]: Exiting...\n");
            strcpy(p->user, "SYSTEM");
            strcpy(p->message, "Exiting...");
            p->notif = 1;
            shmdt(p);
            exit(0);
        }

        strcpy(p->user, user);
        strcpy(p->message, message);
        p->notif = 1;
    }
}

int main() {

    char* name = (char*) malloc(sizeof(char) * 100);
    printf("[SYSTEM] Enter username: ");
    scanf("%[^\n]%*c", name);
    printf("[SYSTEM] Logged in as %s!\n", name);

    int p = fork();

    if (p == 0) listener();
    else sender(name);
    
    return 0;
}