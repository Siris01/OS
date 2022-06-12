#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct free_space {
    struct free_space *next;
    int start;
    int end;
} free_space;

typedef struct allocated_space {
    struct allocated_space *next;
    int start;
    int end;
    char* pid;
} allocated_space;

free_space* free_list;
allocated_space* allocated_list;

int type = 0;
// * 0 = First Fit
// * 1 = Best Fit
// * 2 = Worst Fit

void fix_list() {
    free_space* current = free_list;
    free_space* previous = NULL;

    if (current != NULL && current->start == current->end) {
        free_list = current->next;
        return;
    }

    while (current != NULL && current->start != current->end) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) return;
    previous->next = current->next;

    return;
}

void allocate(char* pid, int size) {
    int start = -1;
    if (type > 0) {
        free_space* temp = free_list;
        int rem = temp->end - temp->start - size; 

        while (temp != NULL) {
            if (type == 1) {
                // * Best Fit *
                int remaining = temp->end - temp->start - size;
                if (remaining > 0 && remaining < rem) {
                    rem = remaining;
                    start = temp->start;
                }
            } else if (type == 2) {
                // * Worst Fit *
                int remaining = temp->end - temp->start - size;
                if (remaining > 0 && remaining > rem) {
                    rem = remaining;
                    start = temp->start;
                }
            }

            temp = temp->next;
        }
    }

    free_space* temp = free_list;

    while (temp != NULL) {
        int free_size = temp->end - temp->start;
        int cond = start > 0 ? (start == temp->start ? 1: 0): 1;

        if (free_size >= size && cond) {
            allocated_space* new_allocated_space = malloc(sizeof(allocated_space));
            new_allocated_space->start = temp->start;
            new_allocated_space->end = temp->start + size;
            new_allocated_space->pid = pid;

            allocated_space* temp_alloc = allocated_list;

            if (temp_alloc != NULL && new_allocated_space->end < temp_alloc->start) {
                new_allocated_space->next = temp_alloc;
                allocated_list = new_allocated_space;
            } else {
                while (temp_alloc != NULL) {

                    if (temp_alloc->end <= new_allocated_space->start) {
                        if (temp_alloc->next == NULL) {
                            new_allocated_space->next = NULL;
                            temp_alloc->next = new_allocated_space;
                            break;

                        } else if (temp_alloc->next->start >= new_allocated_space->end) { 
                            new_allocated_space->next = temp_alloc->next;
                            temp_alloc->next = new_allocated_space;
                            break;

                        }
                    }

                    temp_alloc = temp_alloc->next;
                }
            }
           
            if (allocated_list == NULL) {
                allocated_list = new_allocated_space;
                allocated_list->next = NULL;
            }

            temp->start += size;
            if (temp->start == temp->end) fix_list();

            return;
        }

        temp = temp->next;
    }

    printf("\nUnable to allocate %s\n", pid);
    return;
}

void deallocate(char* pid) {
    allocated_space* temp = allocated_list;

    while (temp != NULL) {
        if (strcmp(temp->pid, pid) == 0) {
            free_space* new_free_space = malloc(sizeof(free_space));
            new_free_space->start = temp->start;
            new_free_space->end = temp->end;
            
            free_space* temp_free = free_list;
            
            if (temp_free != NULL && new_free_space->end < temp_free->start) {
                new_free_space->next = temp_free;
                free_list = new_free_space;
            } else {
                while (temp_free != NULL) {

                    if (temp_free->end <= new_free_space->start) {
                        if (temp_free->next == NULL) {
                            new_free_space->next = NULL;
                            temp_free->next = new_free_space;
                            break;

                        } else if (temp_free->next->start >= new_free_space->end) { 
                            new_free_space->next = temp_free->next;
                            temp_free->next = new_free_space;
                            break;

                        }
                    }

                    temp_free = temp_free->next;
                }
            }
           
            if (free_list == NULL) {
                free_list = new_free_space;
                free_list->next = NULL;
            }

            allocated_space* remove = temp;
            temp = allocated_list;
            allocated_space* previous = NULL;
            while (temp != NULL && temp != remove) {
                previous = temp;
                temp = temp->next;
            }

            if (temp == remove) {
                if (previous == NULL) {
                    allocated_list = temp->next;
                } else {
                    previous->next = temp->next;
                }
            }

            return;
        }

        temp = temp->next;
    }

    printf("\nUnable to find process %s\n", pid);
    return;
}

void display() {
    free_space* temp_free = free_list;
    allocated_space* temp_alloc = allocated_list;

    int free_blocks = 0, alloc_blocks = 0, i = 0;
    while (temp_free != NULL) {
        free_blocks += 1;
        temp_free = temp_free->next;
    }

    while (temp_alloc != NULL) {
        alloc_blocks += 1;
        temp_alloc = temp_alloc->next;
    }

    temp_free = free_list;
    temp_alloc = allocated_list;

    printf("\n\n\nFree Pool:\n");
    for (i = 0; i < free_blocks; i++) printf("---------------");
    printf("\n");
    
    while (temp_free != NULL) {
        printf("|      H      |");
        temp_free = temp_free->next;
    }

    printf("\n");
    for (i = 0; i < free_blocks; i++) printf("---------------");
    printf("\n");

    temp_free = free_list;

    free_space* prev_f = NULL;

    while (temp_free != NULL) {
        if (prev_f == NULL || prev_f->end != temp_free->start) printf("%d", temp_free->start);
        printf("          ");
        printf("%d ", temp_free->end);

        prev_f = temp_free;
        temp_free = temp_free->next;
    }

    printf("\n\nAllocated Pool:\n");
    for (i = 0; i < alloc_blocks; i++) printf("---------------");
    printf("\n");
    
    while (temp_alloc != NULL) {
        printf("|     %s     |", temp_alloc->pid);
        temp_alloc = temp_alloc->next;
    }

    printf("\n");
    for (i = 0; i < alloc_blocks; i++) printf("---------------");
    printf("\n");

    temp_alloc = allocated_list;

    allocated_space* prev_a = NULL;

    while (temp_alloc != NULL) {
        if (prev_a == NULL || prev_a->end != temp_alloc->start) printf("%d", temp_alloc->start);
        printf("          ");
        printf("%d ", temp_alloc->end);

        prev_a = temp_alloc;
        temp_alloc = temp_alloc->next;
    }

    temp_free = free_list;
    temp_alloc = allocated_list;

    printf("\n\nMerged Pool:\n");
    for (i = 0; i < free_blocks + alloc_blocks; i++) printf("---------------");
    printf("\n");

    while (temp_free != NULL && temp_alloc != NULL) {
        if (temp_free->start < temp_alloc->start) {
            printf("|      H      |");
            temp_free = temp_free->next;
        } else {
            printf("|    %s    |", temp_alloc->pid);
            temp_alloc = temp_alloc->next;
        }
    }

    while (temp_free != NULL) {
        printf("|      H      |");
        temp_free = temp_free->next;
    }

    while (temp_alloc != NULL) {
        printf("|    %s    |", temp_alloc->pid);
        temp_alloc = temp_alloc->next;
    }

    printf("\n");
    for (i = 0; i < free_blocks + alloc_blocks; i++) printf("---------------");
    printf("\n");

    temp_free = free_list;
    temp_alloc = allocated_list;
    int prev[100];
    int c = 0;

    while (temp_free != NULL && temp_alloc != NULL) {
        if (temp_free->start < temp_alloc->start) {
            if (prev[c] != temp_free->start) printf("%d", temp_free->start);
            printf("          ");
            printf("%d ", temp_free->end);
            prev[++c] = temp_free->end;

            temp_free = temp_free->next;
        } else {
            if (prev[c] != temp_alloc->start) printf("%d", temp_alloc->start);
            printf("          ");
            printf("%d ", temp_alloc->end);
            prev[++c] = temp_alloc->end;

            temp_alloc = temp_alloc->next;
        }
    }

    while (temp_free != NULL) {
        if (prev[c] != temp_free->start) printf("%d", temp_free->start);
        printf("          ");
        printf("%d ", temp_free->end);
        prev[++c] = temp_free->end;

        prev_f = temp_free;
        temp_free = temp_free->next;
    }

    while (temp_alloc != NULL) {
        if (prev[c] != temp_alloc->start) printf("%d", temp_alloc->start);
        printf("          ");
        printf("%d ", temp_alloc->end);
        prev[++c] = temp_alloc->end;

        prev_a = temp_alloc;
        temp_alloc = temp_alloc->next;
    }

    return;
}

void coalesce() {
    free_space* temp = free_list;
    free_space* previous = NULL;
    int c = 0;

    while (temp != NULL) {
        if (temp->next == NULL) break;

        if (temp->end == temp->next->start) {
            c++;

            temp->end = temp->next->end;
            temp->next = temp->next->next;
        }
        
        temp = temp->next;
    }
    
    printf("\n%d free spaces coalesced\n", c);
    return;
}

void init(int blocks, int start[], int end[]) {
    int i = 0;
    allocated_list = NULL;
    free_list = (free_space*) malloc(sizeof(free_space));

    for (i = 0; i < blocks; i++) {
        free_space* temp = (free_space*) malloc(sizeof(free_space));

        temp->start = start[i];
        temp->end = end[i];
        temp->next = NULL;
        
        if (i == 0) free_list = temp;
        else {
            free_space* temp_free = free_list;

            while (temp_free->next != NULL) {
                temp_free = temp_free->next;
            }
            temp_free->next = temp;
        }
    }

    printf("\nAll blocks of memory are initialized.\n");
    return;
}

int menu() {
    char ch;
    printf("\n\nChoose an option:\n");
    printf("1. Allocate\n");
    printf("2. Deallocate\n");
    printf("3. Display\n");
    printf("4. Coalesce holes\n");
    printf("5. Exit\n");
    
    scanf("\n%c", &ch);
    printf("\n");
    return  ch - '0';
}

void test(int *blocks, int start[], int end[]) {
    *blocks = 4;
    start[0] = 100;
    start[1] = 110;
    start[2] = 115;
    start[3] = 118;

    end[0] = 110;
    end[1] = 115;
    end[2] = 118;
    end[3] = 125;

    type = 0;   

    init(*blocks, start, end);

    allocate("P01", 4);
    allocate("P02", 4);
    allocate("P03", 5);
    display();

    deallocate("P01");
    display();

    deallocate("P02");
    display();

    coalesce();
    display();
    return;
}

int main() {
    int blocks = 0, i = 0;
    int start[100], end[100];
    
    /*
    printf("\nEnter the type of allocation:\n");
    printf("1. First fit\n");
    printf("2. Best fit\n");
    printf("3. Worst fit\n");
    scanf("\n%d", &type);
    type--;
    
    printf("Enter the number of blocks: ");
    scanf("%d", &blocks);

    for (i = 0; i < blocks; i++) {
        printf("Enter the starting address of block %d: ", i+1);
        scanf("%d", &start[i]);
        printf("Enter the ending address of block %d: ", i+1);
        scanf("%d", &end[i]);
    }
    */

    test(&blocks, start, end);
    return 0;

    init(blocks, start, end);

    while (1) {
        int ch = menu();

        switch (ch) {
            case 1:
                char* pid1 = malloc(sizeof(char) * 10);
                int size = 0;

                printf("Enter the pid: ");
                scanf("%s", pid1);

                printf("Enter the size of %s: ", pid1);
                scanf("%d", &size);

                allocate(pid1, size);
                break;
            case 2:
                char* pid2 = malloc(sizeof(char) * 10);

                printf("Enter the pid: ");
                scanf("%s", pid2);

                deallocate(pid2);
                break;
            case 3:
                display();
                break;
            case 4:
                coalesce();
                break;
            default:
                exit(0);
        }
    }

    return 0;
}