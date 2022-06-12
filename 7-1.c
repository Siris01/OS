#include <stdio.h>
#include <stdlib.h>
#define MAX 25

int alloc[MAX][MAX];
int max[MAX][MAX];
int need[MAX][MAX];
int avail[MAX];
int process_count = 0;
int resource_count = 0;

int bankers(int suppress_output) {
    int n = process_count, m = resource_count, i = 0, j = 0, k = 0;
    int f[MAX], ans[MAX], ind = 0;

    for (k = 0; k < n; k++) f[k] = 0;
    
    int y = 0;
    for (k = 0; k < 5; k++) {
        for (i = 0; i < n; i++) {
            if (f[i] == 0) {
 
                int flag = 0;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]){
                        flag = 1;
                         break;
                    }
                }
 
                if (flag == 0) {
                    ans[ind++] = i;
                    for (y = 0; y < m; y++)
                        avail[y] += alloc[i][y];
                    f[i] = 1;
                }
            }
        }
    }
   
    int flag = 1;
       
    for(int i=0;i<n;i++) {
        if(f[i]==0) {
            flag=0;
            if (!suppress_output) printf("The following system is not safe");
            return -1;
        }
    }

    if (suppress_output) return 1; 
     
    if (flag == 1) {
        printf("The system is in a safe state!\n");
        printf("Following is the SAFE Sequence\n");
        for (i = 0; i < n - 1; i++)
            printf(" P%d ->", ans[i]);
        printf(" P%d", ans[n - 1]);
    }

    return 1;
}

void req() {

    int process = 0, request[MAX], i;

    printf("Enter the process ID: ");
    scanf("%d", &process);

    for (i = 0; i < resource_count; i++) {
        printf("Enter request for resource R%d: ", i);
        scanf("%d", &request[i]);
    }

    for (i = 0; i < resource_count; i++) {
        if (request[i] > need[process][i]) {
            printf("Request of R%d is greater than Need!", i);
            return;
        }

        if (request[i] > avail[i]) {
            printf("Request of R%d is greater than available!", i);
            return;
        }
    }

    for (i = 0; i < resource_count; i++) {
        avail[i] -= request[i];
        need[process][i] -= request[i];
        alloc[process][i] += request[i];
    }

    int is_safe = bankers(1);

    if (is_safe) printf("Requested resources have been granted!");
    else {
        printf("Requested resources cannot be granted");

        for (i = 0; i < resource_count; i++) {
            avail[i] += request[i];
            need[process][i] += request[i];
            alloc[process][i] -= request[i];
        }
    }

    return;
}

void print_data() {
    int i = 0, j = 0;

    printf("\n\n        ALLOC    \t\t     MAX    \t\t    NEED    \t\t    AVAIL    \n");
    printf("       ");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < resource_count; j++) {
            printf("R%d ", j);
        }
        printf("\t\t  ");
    }
    printf("\n");

    for (i = 0; i < process_count; i++) {
        printf("P%d     ", i);
        for (j = 0; j < resource_count; j++) {
            printf("%d  ", alloc[i][j]);
        }
        printf("\t\t  ");

        for (j = 0; j < resource_count; j++) {
            printf("%d  ", max[i][j]);
        }
        printf("\t\t  ");

        for (j = 0; j < resource_count; j++) {
            printf("%d  ", need[i][j]);
        }
        printf("\t\t  ");

        if (i == 0) {
            for (j = 0; j < resource_count; j++) {
                printf("%d  ", avail[j]);
            }
        }

        printf("\n");
    }
    
    return;
}

void init() {
    int i = 0, j = 0;

    process_count = 5;
    resource_count = 3;

    avail[0] = 3;
    avail[1] = 3;
    avail[2] = 2;

    alloc[0][0] = 0;
    alloc[0][1] = 1;
    alloc[0][2] = 0;
    alloc[1][0] = 2;
    alloc[1][1] = 0;
    alloc[1][2] = 0;
    alloc[2][0] = 3;
    alloc[2][1] = 0;
    alloc[2][2] = 2;
    alloc[3][0] = 2;
    alloc[3][1] = 1;
    alloc[3][2] = 1;
    alloc[4][0] = 0;
    alloc[4][1] = 0;
    alloc[4][2] = 2;

    max[0][0] = 7;
    max[0][1] = 5;
    max[0][2] = 3;
    max[1][0] = 3;
    max[1][1] = 2;
    max[1][2] = 2;
    max[2][0] = 9;
    max[2][1] = 0;
    max[2][2] = 2;
    max[3][0] = 2;
    max[3][1] = 2;
    max[3][2] = 2;
    max[4][0] = 4;
    max[4][1] = 3;
    max[4][2] = 3;

    /*
    printf("Enter the number of processes: ");
    scanf("%d", &process_count);

    printf("Enter the number of resources: ");
    scanf("%d", &resource_count);

    for (i = 0; i < resource_count; i++) {
        printf("Enter the number of instances for resource R%d: ", i);
        scanf("%d", &avail[i]);
    }

    for (i = 0; i < process_count; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < resource_count; j++) {
            printf("Enter the max need of R%d: ", j);
            scanf("%d", &max[i][j]);
        }
    }

    for (i = 0; i < process_count; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < resource_count; j++) {
            printf("Enter the allocation of R%d: ", j);
            scanf("%d", &alloc[i][j]);
        }
    }
    */

    for (i = 0; i < process_count; i++) {
        for (j = 0; j < resource_count; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    return;
}

int menu () {
    int ch;

    printf("\n\nEnter an option:\n");
    printf("1. Print Data\n");
    printf("2. Check system state\n");
    printf("3. Resource Request\n");
    printf("4. Exit\n");

    scanf("\n%d", &ch);

    return ch;
}

int main() {
    init();
     
    while (1) {
        int ch = menu();

        switch (ch) {
            case 1:
                print_data();
                break;
            case 2:
                bankers(0);
                break;
            case 3:
                req();
                break;
            case 4:
                exit(0);
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }

    return 0;
}