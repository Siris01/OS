#include <stdio.h>

void deadlock(int n, int m, int avail[], int max[][10], int alloc[][10], int request[][10]){
  int is_safe = 1;
      int f[10], ans[10], w[10], ind = 0, i,j,pid;
        for(i=0; i<n;i++){
          for(j=0;i<m;i++){  
            if(alloc[i][j]!=0)
              f[i] = 0;
          }
          if(j==3){
            f[i] = 1;
          }
        }


      for(i = 0;  i  < m; i++) {
        w[i] = avail[i];
      }
    
      int y = 0;
      for (int k = 0; k < 5; k++) {
        for (i = 0; i < n; i++) {
          if (f[i] == 0) {

            int flag = 0;
            for (j = 0; j < m; j++) {
              if (request[i][j] > w[j]) {
                flag = 1;
                break;
              }
            }

            if (flag == 0) {
              ans[ind++] = i;
              for (y = 0; y < m; y++)
                w[y] += alloc[i][y];
              f[i] = 1;
            }
          }
        }
      }


      for (int i = 0; i < n; i++) {
        if (f[i] == 0) {
          printf("P%d is deadlocked\n", i);
          is_safe = 0;
        }
      }

      
    if (is_safe) printf("System is in safe state!");

    return;
}

void disp(int n, int m, int avail[], int max[][10], int alloc[][10], int need[][10]){
    printf("Pid\tAlloc\tMax  \tNeed \tAvail\t\nR-\t");
      for(int i=0; i < m; i++) {
        printf("%d ",i);
      }
      printf("\t");
      for(int i=0; i < m; i++) {
        printf("%d ",i);
      }
      printf("\t");
      for(int i=0; i < m; i++) {
        printf("%d ",i);
      }
      printf("\t");
      printf("\n----------------------------------\n");
      for (int i = 0; i < n; i++) {
        printf("P%d\t",i);
        for (int j = 0; j < m; j++)
          printf("%d ", alloc[i][j]);
        printf("\t");
        for (int j = 0; j < m; j++)
          printf("%d ", max[i][j]);
        printf("\t");
        for (int j = 0; j < m; j++)
          printf("%d ", need[i][j]);
        printf("\t");
        if(i==0)
          for (int j = 0; j < m; j++)
            printf("%d ", avail[j]);
        printf("\t\n");
      }
}

int main() {
  // P0, P1, P2, P3, P4 are the Process names here

  int n, m, i, j, k;
  n = 5; // Number of processes
  m = 3; // Number of resources
  int need[10][10];
  int request[10][10];
  int alloc[10][10]; 
  int max[10][10];
  int avail[10];  //3 3 2
  int choice;

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
      printf("Number of processes:");
      scanf("%d", & n);
      printf("Number of resources:");
      scanf("%d", & m);
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
          need[i][j] = 0;
      }
      printf("Enter availability matrix:");
      for (int i = 0; i < m; i++) {
        scanf("%d", & avail[i]);
      }
      printf("Enter max matrix:");
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
          scanf("%d", & max[i][j]);
      }

      printf("Enter allocation matrix:");
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
          scanf("%d", & alloc[i][j]);
      }
  
*/

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
     
    disp(n,m,avail,max,alloc,need);
      

      printf("Enter the request matrix: ");
    for(i=0;i<n;i++)  
      for(k=0;k<m;k++)
        scanf("%d",&request[i][k]);
    
      printf("\n");
     deadlock(n,m,avail,max,alloc,request);

  return (0);
}

