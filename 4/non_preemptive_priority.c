#define totalprocess 5
#include <stdio.h>
#include<stdlib.h>

struct process {
    int at, bt, pr, pno;
}
proc[50];

int comp(const void * a, const void * b) {
    int a_at = ((struct process *)a)->at;
    int b_at = ((struct process *)b)->at;

    int a_pr = ((struct process *)a)->pr;
    int b_pr = ((struct process *)b)->pr;

    if (a_at == b_at) return a_pr < b_pr ? 1 : -1;
    else return a_at < b_at ? 1 : -1;
}

void get_wt_time(int wt[]) {
    int service[50];

    service[0] = proc[0].at;
    wt[0] = 0;

    for (int i = 1; i < totalprocess; i++) {
        service[i] = proc[i - 1].bt + service[i - 1];
        wt[i] = service[i] - proc[i].at;

        if (wt[i] < 0) {
            wt[i] = 0;
        }
    }

}

void get_tat_time(int tat[], int wt[]) {
    for (int i = 0; i < totalprocess; i++) {
        tat[i] = proc[i].bt + wt[i];
    }

}

void findgc() {
    int wt[50], tat[50];
    double wavg = 0, tavg = 0;

    get_wt_time(wt);
    get_tat_time(tat, wt);

    int stime[50], ctime[50];

    stime[0] = proc[0].at;
    ctime[0] = stime[0] + tat[0];

    for (int i = 1; i < totalprocess; i++) {
        stime[i] = ctime[i - 1];
        ctime[i] = stime[i] + tat[i] - wt[i];
    }

    printf("PNO\tStart Time\tEnd Time\tTurnaround Time\tWaiting Time\n");

    for (int i = 0; i < totalprocess; i++) {
        wavg += wt[i];
        tavg += tat[i];

        printf("%d\t%d\t%d\t%d\t%d\n", proc[i].pno, stime[i], ctime[i], tat[i], wt[i]);
    }

    printf("Average Waiting Time: %.2f\n", wavg / totalprocess);
    printf("Average Turnaround Time: %.2f\n", tavg / totalprocess);
}

int main() {
    int arrivaltime[] = {
        1,
        2,
        3,
        4,
        5
    };
    int bursttime[] = {
        3,
        5,
        1,
        7,
        4
    };
    int priority[] = {
        3,
        4,
        1,
        7,
        8
    };

    for (int i = 0; i < totalprocess; i++) {
        proc[i].at = arrivaltime[i];
        proc[i].bt = bursttime[i];
        proc[i].pr = priority[i];
        proc[i].pno = i + 1;
    }

    qsort((void * )(proc + 1), totalprocess, sizeof(proc[1]), comp);
    findgc();
    return 0;
}