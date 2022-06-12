#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_COUNT 5

void ChildProcess();
void ParentProcess();
void isPrime(int num);

int num = 0;

void isPrime(int num) {
    int c = 0;
    for (int i = num; i >=1; i) {
        if (num % i == 0) c++;
        if (c>2) {
        printf("%d Is not a prime number", num);
        return;
        }
    } 
    printf("%d Is a prime number", num);
}

int main() {
    printf("Enter value of number: ");
    scanf("%d", &num);
    pid_t pid;
    pid = fork();
    if(pid==0) ChildProcess();
    else ParentProcess();
}

void ChildProcess() {

    int i;
    for (i=1; i<=MAX_COUNT; i++) printf("This line is from child, value =%d\n",i);
    printf("*** Child process is done ***\n");
    isPrime(num);
}

void ParentProcess() {
    int i;
    for (i=1; i<=MAX_COUNT; i++) printf("This line is from parent, value =%d\n",i);
    printf("*** Parent is done ***\n");
}