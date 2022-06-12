#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

int reference[MAX];
int ref_count;
typedef struct frames_s {
    struct frames_s *next;
    int page_number;
} frames_s;

frames_s* frames;
int free_frames;
int req_frames;

void init_list() {
    int i = 0;
    frames = (frames_s*) malloc(sizeof(frames_s));

    for (i = 0; i < req_frames; i++) {
        frames_s* node = (frames_s*) malloc(sizeof(frames_s));
        node->page_number = -1;
        node->next = NULL;
        
        if (i == 0) frames = node;
        else {
            frames_s* temp = frames;
            while (temp->next != NULL) temp = temp->next;
            temp->next = node;
        }
    }
}

void read() {

    printf("Enter number of free frames: ");
    scanf("%d", &free_frames);

    printf("Enter number of required frames: ");
    scanf("%d", &req_frames);

    char* reference_string = malloc(MAX);
    char temp;
    printf("Enter reference string: ");
    scanf("%c",&temp); 
    scanf("%[^\n]", reference_string);

    ref_count = 0; 
    char* token = strtok(reference_string, " ");
    while( token != NULL ) {
        reference[ref_count++] = atoi(token);
        token = strtok(NULL, " ");
    }

    printf("Data read successfully!\n");
}

void fifo() {
    int page_faults = 0, i, j, flag, first = 0;
    init_list();

    printf("PAGE\tMEM\t\tPF\n");

    for (i = 0; i < ref_count; i++) {
        frames_s* temp = frames;
        flag = 0;

        while (temp != NULL) {
            if (temp->page_number == reference[i]) {
                flag = 1;
                break;
            }
            temp = temp->next;
        }

        temp = frames;
        printf("%d\t", reference[i]);

        if (flag) {
            while (temp != NULL) {
                if (temp->page_number >= 0) printf("%d ", temp->page_number);
                else printf("- ");
                temp = temp->next;
            }

            printf("\t%d\n", page_faults);
            continue;
        }
    
        temp = frames;
        for (j = 0; j < first; j++) temp = temp->next;

        temp->page_number = reference[i];
        first = (first + 1) % req_frames;
        page_faults++;
        temp = frames;

        while (temp != NULL) {
            if (temp->page_number >= 0) printf("%d ", temp->page_number);
            else printf("- ");
            temp = temp->next;
        }

        printf("\t%d\n", page_faults);
    }

    printf("\nTotal Number of page faults in FIFO: %d\n", page_faults);
    return;
}

void optimal() {
    int page_faults = 0, i, j, flag, indices[MAX/10], replace = 0, k = 0;
    init_list();
    printf("PAGE\tMEM\t\tPF\n");

    for (i = 0; i < ref_count; i++) {
        frames_s* temp = frames;
        flag = 0;

        while (temp != NULL) {
            if (temp->page_number == reference[i]) {
                flag = 1;
                break;
            }
            temp = temp->next;
        }
        
        temp = frames;
        printf("%d\t", reference[i]);

        if (flag) {
            while (temp != NULL) {
                if (temp->page_number >= 0) printf("%d ", temp->page_number);
                else printf("- ");
                temp = temp->next;
            }

            printf("\t%d\n", page_faults);
            continue;
        }
        
        temp = frames;
        k = 0;
        while (temp != NULL) {
            for (j = i; j < ref_count; j++) {
                if (temp->page_number == reference[j]) {
                    indices[k++] = j;
                    break;
                }
            }
            if (j == ref_count) indices[k++] = MAX * 10;
            temp = temp->next;
        }
        
        replace = 0;
        for (j = 0; j < k; j++) {
            if (indices[j] > indices[replace]) replace = j;
        }
        
        temp = frames;
        for (j = 0; j < replace; j++) temp = temp->next;
        if (page_faults < req_frames) {
            temp = frames;
            while (temp->page_number != -1) temp = temp->next;
        }
        temp->page_number = reference[i];
        page_faults++;
        temp = frames;

        while (temp != NULL) {
            if (temp->page_number >= 0) printf("%d ", temp->page_number);
            else printf("- ");
            temp = temp->next;
        }

        printf("\t%d\n", page_faults);
    }

    printf("\nTotal Number of page faults in FIFO: %d\n", page_faults);
    return;
}

void lru() {
    int page_faults = 0, i, j, flag, indices[MAX/10], replace = 0, k = 0;
    init_list();
    printf("PAGE\tMEM\t\tPF\n");

    for (i = 0; i < ref_count; i++) {
        frames_s* temp = frames;
        flag = 0;

        while (temp != NULL) {
            if (temp->page_number == reference[i]) {
                flag = 1;
                break;
            }
            temp = temp->next;
        }
        
        temp = frames;
        printf("%d\t", reference[i]);

        if (flag) {
            while (temp != NULL) {
                if (temp->page_number >= 0) printf("%d ", temp->page_number);
                else printf("- ");
                temp = temp->next;
            }

            printf("\t%d\n", page_faults);
            continue;
        }
        
        temp = frames;
        k = 0;
        while (temp != NULL) {
            for (j = 0; j < i; j++) {
                if (temp->page_number == reference[j]) indices[k] = j;
            }
            k++;
            temp = temp->next;
        }
        
        replace = 0;
        for (j = 0; j < k; j++) {
            if (indices[j] < indices[replace]) replace = j;
        }
        
        temp = frames;
        for (j = 0; j < replace; j++) temp = temp->next;
        if (page_faults < req_frames) {
            temp = frames;
            while (temp->page_number != -1) temp = temp->next;
        }
        temp->page_number = reference[i];
        page_faults++;
        temp = frames;

        while (temp != NULL) {
            if (temp->page_number >= 0) printf("%d ", temp->page_number);
            else printf("- ");
            temp = temp->next;
        }

        printf("\t%d\n", page_faults);
    }

    printf("\nTotal Number of page faults in FIFO: %d\n", page_faults);
    return;
}

void lfu() {

}

int menu() {
    char ch;

    printf("\n\nMenu:\n");
    printf("1. Read data\n");
    printf("2. FIFO\n");
    printf("3. Optimal\n");
    printf("4. LRU\n");
    printf("5. LFU\n");
    printf("6. Exit\n");

    scanf("\n%c", &ch);
    printf("\n");
    return ch - '0';
}

int main() {
    int choice = -1;
    while (1) {
        choice = menu();
        switch (choice) {
            case 0:
                break;
            case 1:
                read();
                break;
            case 2:
                fifo();
                break;
            case 3:
                optimal();
                break;
            case 4:
                lru();
                break;
            case 5:
                lfu();
                break;
            default:
                exit(0);
        }
    }

    return 0;
}

