#define _GNU_SOURCE  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

char *page[MAX];
int pages = 0;
int total_size = 0;
int page_size = 0;
int process_count = 0;
char *proccess[MAX];

void alloc(char *process, int size) {
    int i = 0, c = 0;

    for (i = 0; i < pages; i++) {
        if (strcmp(page[i], "") == 0) c++;
    }

    if (c < size/page_size) {
        printf("Not enough memory\n");
        return;
    }

    int alloc[MAX], j = 0;

    for (i = 0; i < pages; i++) {
        if (size/page_size == j) break;
        if (strcmp(page[i], "") == 0) {
            strcpy(page[i], process);
            alloc[j++] = i;
        }
    }

    strcpy(proccess[process_count], process);
    process_count++;

    printf("Process is divided into %d pages\n", size/page_size);
    printf("Allocation:\n");

    for (i = 0; i < j; i++) printf("Page %d: Frame %d\n", i, alloc[i]);
    
}

void dealloc(char *process) {
    int i = 0, c = 0;

    for (i = 0; i < pages; i++) {
        if (strcmp(page[i], process) == 0) {
            strcpy(page[i], "");
            c++;
        }
    }

    if (c == 0) {
        printf("Process not found\n");
        return;
    }

    for (i = 0; i < process_count; i++) {
        if (strcmp(proccess[i], process) == 0) {
            strcpy(proccess[i], "");
            break;
        }
    }
    process_count--;

    printf("Process %s is deallocated\n", process);
    printf("%d frames were freed\n", c);
}

void print() {
    char* processes[MAX];
    int count[MAX], i = 0, c = 0, j = 0;

    for (i = 0; i < MAX; i++) {
        if (strcmp(proccess[i], "") == 0) continue;

        printf("\nPage table for %s\n", proccess[i]);
        c = 0;

        for (j = 0; j < pages; j++) {
            if (strcmp(page[j], proccess[i]) == 0) {
                printf("Page %d: Frame %d\n", c++, j);
            }
        }
    }    
}

void free_list() {
    int i = 0;
    print("Free frames: ");

    for (i = 0; i < pages; i++) {
        if (strcmp(page[i], "") == 0) printf("%d ", i);
    }

    printf("\n");
}

void init() {
    int ts = 0;
    int ps = 0;

    printf("Enter total size of memory: ");
    scanf("%d", &ts);
    printf("Enter page size: ");
    scanf("%d", &ps);

    total_size = ts;
    page_size = ps;
    pages = ts/ps;

    srand(time(NULL));
    for (int i = 0; i < pages; i++) {
        page[i] = malloc(sizeof(char) * MAX);
        if (rand() % 4 != 0) strcpy(page[i], "");
        else {
            char* num;
            char buffer[MAX];

            strcpy(page[i], "junk");
        } 
    }

    for (int i = 0; i < MAX; i++) proccess[i] = malloc(sizeof(char) * MAX);

    process_count++;
    strcpy(proccess[0], "junk");
}

int menu() {
    int ch;

    printf("\n\nEnter an option:\n");
    printf("1. Process request\n");
    printf("2. Deallocation\n");
    printf("3. Display page table for all processes\n");
    printf("4. List free frames\n");
    printf("5. Exit\n");

    scanf("\n%d", &ch);

    return ch;
}

int main() {
    init();

    while (1) {
        int choice = menu();
        switch (choice) {
            case 1:
                char* p; int sz = 0;
                p = malloc(sizeof(char) * MAX);
                printf("Enter process name: ");
                scanf("%s", p);
                printf("Enter size: ");
                scanf("\n%d", &sz);
                alloc(p, sz);
                break;
            case 2:
                char* p1 = malloc(sizeof(char) * MAX);
                printf("Enter process name: ");
                scanf("%s", p1);
                dealloc(p1);
                break;
            case 3:
                print();
                break;
            case 4:
                free_list();
                break;
            default:
                exit(0);
        }
    }

    return 0;
}

