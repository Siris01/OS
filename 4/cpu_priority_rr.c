#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct times {
    int pid, at, bt, wt, ta, remaining, nat;
};

typedef struct process {
    char PID[20];
    int arrival;
    int burst;
    int wait;
    int turnaround;
    int service;
    int priority;
} process;

typedef struct gantt {
    char PID[20];
    int time;
} gantt;

void sortart(struct times a[], int pro) {
    int i, j;
    struct times temp;
    for (i = 0; i < pro; i++) {
        for (j = i + 1; j < pro; j++) {
            if (a[i].at > a[j].at) {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
    return;
}

void ganttchart(gantt * G, process * P, int tp) {
    int i, j;
    int tot = 0;
    printf("\nGantt Chart\n ");
    for (i = 0; i < tp; i++) {
        for (j = 0; j < G[i].time; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n\b| ");
    for (i = 0; i < tp; i++) {
        for (j = 0; j < G[i].time - 1; j++) printf(" ");
        printf("%s", G[i].PID);
        for (j = 0; j < G[i].time - 1; j++) printf(" ");
        printf(" |");
    }
    printf("\n ");
    for (i = 0; i < tp; i++) {
        for (j = 0; j < G[i].time; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n0");
    for (i = 0; i < tp; i++) {
        for (j = 0; j < G[i].time; j++) printf("  ");
        if (G[i].time > 9) printf("\b");
        if (G[i].time > 19) printf("\b ");
        tot += G[i].time;
        printf("\b %d", tot);
    }
    printf("\n");
}

float avgburst(process * P, int size) {
    float bt;
    for (int i = 0; i < size; i++) {
        bt += P[i].burst;
    }
    return bt / size;
}

float avgtat(process * P, int size) {
    float tat;
    for (int i = 0; i < size; i++) {
        tat += P[i].turnaround;
    }
    return tat / size;
}

double avgwt(process * P, int size) {
    double wt;
    for (int i = 0; i < size; i++) {
        wt += P[i].wait;
    }
    return wt / size;
}

void waiting_time(process * P, int n) {
    P[0].wait = 0;
    P[0].service = P[0].arrival;
    for (int i = 1; i < n; i++) {
        P[i].service = P[i - 1].service + P[i - 1].burst;
        P[i].wait = P[i].service - P[i].arrival;
    }
    return;
}

void turnaround_time(process * P, int n) {
    for (int i = 0; i < n; i++) {
        P[i].turnaround = P[i].wait + P[i].burst;
    }
    return;
}

void priority() {
    int n;
    printf("Pre-Emptive Priority based job Job Scheduler\nEnter number of processes:\t");
    scanf(" %d", & n);
    process P[n + 1];
    int temp[n];
    double jus, l;
    int tp = 0;
    gantt G[50];
    int shortp, count = 0;
    printf("Input:\n");
    for (int i = 0; i < n; i++) {
        printf("PID %d: ", i + 1);
        scanf("%s", P[i].PID);
        printf("Arrival time %d: ", i + 1);
        scanf("%d", & P[i].arrival);
        printf("Burst time %d: ", i + 1);
        scanf("%d", & P[i].burst);
        printf("Priority %d: ", i + 1);
        scanf("%d", & P[i].priority);
        temp[i] = P[i].burst;
    }

    P[n].priority = 9999;

    for (int t = 0; count != n; t++) {
        shortp = n;
        for (int i = 0; i < n; i++) {
            if (P[i].priority < P[shortp].priority && (P[i].arrival <= t && P[i].burst > 0)) shortp = i;
        }

        P[shortp].burst = P[shortp].burst - 1;
        if (P[shortp].burst == 0) {
            count++;
            P[shortp].wait = t + 1 - P[shortp].arrival - temp[shortp];
            P[shortp].turnaround = t + 1 - P[shortp].arrival;
        }
        if (tp == 0) {
            strcpy(G[tp].PID, P[shortp].PID);
            G[tp].time = 1;
            tp++;
        } else {
            if (strcmp(P[shortp].PID, G[tp - 1].PID) == 0)
                G[tp - 1].time += 1;
            else {
                strcpy(G[tp].PID, P[shortp].PID);
                G[tp].time = 1;
                tp++;
            }
        }
    }
    printf("Pre-Emptive Job Scheduling Priority\n");
    printf("\n*********************************************\n");

    printf("PID\tA_time\tB_time\tW_time\tT_time\tPriority\n");
    printf("\n*********************************************\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\n", P[i].PID, P[i].arrival, temp[i], P[i].wait, P[i].turnaround, P[i].priority);
    }
    printf("\n\nAvg TAT: %.2f\tAvg WT: %.2f\n", avgtat(P, n), avgwt(P, n));
    ganttchart(G, P, tp);
}

void roundrobin() {
    int i, j, pro, time, remain, flag = 0, q;
    struct times a[100];
    float avgwt = 0, avgtt = 0;
    printf("Round Robin Scheduling Algorithm\n");
    
    printf("Enter Number Of Processes : ");
    scanf("%d", & pro);
    remain = pro;
    printf("Enter Burst time for Processes P1 to P%d", pro);
    for (i = 0; i < pro; i++) {
        a[i].at = 0;
        scanf("%d", & a[i].bt);
        a[i].pid = i;
        a[i].remaining = a[i].bt;
    }
    sortart(a, pro);
    printf("Enter Time Slice OR Quantum Number : ");
    scanf("%d", & q);
    printf("\n***************************************\n");
    printf("Gantt Chart\n");
    printf("0");

    for (time = 0, i = 0; remain != 0;) {
        if (a[i].remaining <= q && a[i].remaining > 0) {

            time = time + a[i].remaining;

            printf(" -> [P%d] <- %d", a[i].pid + 1, time);
            a[i].remaining = 0;
            flag = 1;
        } else if (a[i].remaining > 0) {
            a[i].remaining = a[i].remaining - q;
            time = time + q;
            printf(" -> [P%d] <- %d", a[i].pid + 1, time);
        }
        if (a[i].remaining == 0 && flag == 1) {
            remain--;
            a[i].ta = time - a[i].at;
            a[i].wt = time - a[i].at - a[i].bt;
            avgwt = avgwt + time - a[i].at - a[i].bt;
            avgtt = avgtt + time - a[i].at;
            flag = 0;
        }
        if (i == pro - 1)
            i = 0;
        else if (a[i + 1].at <= time)
            i++;
        else
            i = 0;

    }

    printf("\n\n");
    printf("**********************************\n");

    printf("PIDs  A_Time  B_Time  W_Time T_Time\n");
    printf("**********************************\n");
    for (i = 0; i < pro; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", a[i].pid + 1, a[i].at, a[i].bt, a[i].wt, a[i].ta);
    }
    printf("**********************************\n");
    avgwt = avgwt / pro;
    avgtt = avgtt / pro;
    printf("Average Waiting Time : %.2f\n", avgwt);
    printf("Average Turnaround Time : %.2f\n", avgtt);
}

int main() {

    int c;

    do {
        printf("\tCPU SCHEDULING\n1. Priority -Preemptive\n2. Round Robin\n3. Exit\n");
        printf("Enter your choice:\n");
        scanf("%d", & c);
        if (c == 1) {
            priority();
        } else if (c == 2) {
            roundrobin();
        } else if (c == 3) {
            printf("Bye!\n");
            break;
        } else {
            printf("Invalid option\n");
        }

    } while (1);
}

/*
P1 1 5 4
P2 2 2 5
P3 3 6 6
P4 0 4 1
P5 4 7 2
P6 5 8 3
*/