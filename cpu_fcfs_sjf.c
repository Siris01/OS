#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct process {
    char process_id[5];
    int burst_time, arrival_time, active, remaining_time, waiting_time, turnaround_time;
} process;

char time[1000][5];
int maxt;

void print_gantt_chart(process p[], int n) {
    puts("GANTT CHART:");
    int cur = 0, cnt = 1, size = 0;

    for (int i = 1; i <= maxt; i++) {
        if (!strcmp(time[i], time[i - 1])) {
            cnt++;
        } else {
            for (int j = 0; j < cnt; j++) {
                size++;
            }
            size++;
            for (int j = 1; j < cnt; j++) {
                size++;
            }
            cnt = 1;
            size++;
        }
    }

    for (int i = 0; i < size + 5; i++) {
        printf("-");
    }
    printf("\n|");
    cur = 0, cnt = 1;

    for (int i = 1; i <= maxt; i++) {
        if (!strcmp(time[i], time[i - 1])) {
            cnt++;
        } else {
            for (int j = 0; j < cnt; j++) {
                printf(" ");
            }
            printf("%s", time[i - 1]);
            for (int j = 1; j < cnt; j++) {
                printf(" ");
            }
            cnt = 1;
            printf(" |");
        }
    }

    printf("\n");
    for (int i = 0; i < size + 5; i++) {
        printf("-");
    }
    printf("\n");
    printf("0");

    cur = 0, cnt = 1;
    for (int i = 1; i <= maxt; i++) {
        if (!strcmp(time[i], time[i - 1])) {
            cnt++;
        } else {
            printf(" ");
            for (int j = 0; j < cnt; j++) {
                printf("  ");
            }
            cur += cnt;
            cnt = 1;
            printf("%2d", cur);
        }
    }
    puts("\n");
}

void print_table(process p[], int n) {
    int i;
    puts("\nTABLE:");

    puts("--------------------------------------------------------------------");
    puts("| PID | Arrival Time | Burst Time | Waiting Time | Turnaround Time |");
    puts("--------------------------------------------------------------------");

    for (i = 1; i <= n; i++) {
        printf("| %s  |     %2d       |     %2d     |      %2d      |        %2d       |\n", p[i].process_id, p[i].arrival_time, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time);
        puts("--------------------------------------------------------------------");
    }

}

int cmp_burst_time(const void * p, const void * q) {
    int b1 = ((struct process*)p)->burst_time;
    int b2 = ((struct process*)q)->burst_time;
    if (b1 < b2) return -1;
    return 1;
}

int cmp_arrival_time(const void * p, const void * q) {
    int a1 = ((struct process * ) p) -> arrival_time;
    int a2 = ((struct process * ) q) -> arrival_time;
    if (a1 < a2) return -1;
    return 1;
}

void fcfs() {
    int number_of_processes;
    process p[100];
    int curr_time = 0;
    double avg_wt, avg_tt;
    int sum_wt = 0, sum_tt = 0;

    printf("FCFS CPU SCHEDULER\n");
    printf("Number of Processes: ");
    scanf("%d", & number_of_processes);

    for (int i = 1; i <= number_of_processes; i++) {
        printf("Process ID: ");
        scanf("%s", p[i].process_id);
        printf("arrival Time: ");
        scanf("%d", & p[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", & p[i].burst_time);
    }

    qsort((void * )(p + 1), number_of_processes, sizeof(p[1]), cmp_arrival_time);

    for (int i = 1; i <= number_of_processes; i++) {
        p[i].waiting_time = curr_time - p[i].arrival_time;
        if (p[i].waiting_time < 0) p[i].waiting_time = 0;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        if (curr_time < p[i].arrival_time) {
            curr_time = p[i].arrival_time + p[i].burst_time;
            for (int j = p[i].arrival_time; j < curr_time; j++) {
                strcpy(time[j], p[i].process_id);
            }
        } else {
            curr_time = curr_time + p[i].burst_time;
            for (int j = 1; j <= p[i].burst_time; j++) {
                strcpy(time[curr_time - j], p[i].process_id);
            }
        }
        sum_wt += p[i].waiting_time;
        sum_tt += p[i].turnaround_time;
    }

    maxt = curr_time;
    print_table(p, number_of_processes);
    avg_wt = 1.0l * sum_wt / number_of_processes;
    avg_tt = 1.0l * sum_tt / number_of_processes;

    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n\n\n", avg_tt);
    print_gantt_chart(p, number_of_processes);
}

void sjf_np() {
    int number_of_processes;
    process p[100];
    int turnaround_time, waiting_time;
    int curr_time = 0;
    double avg_wt, avg_tt;
    int sum_wt = 0, sum_tt = 0;

    printf("Number of Processes: ");
    scanf("%d", & number_of_processes);

    for (int i = 1; i <= number_of_processes; i++) {
        printf("Process ID: ");
        scanf("%s", p[i].process_id);
        printf("Burst Time: ");
        scanf("%d", & p[i].burst_time);
        p[i].arrival_time = 0;
    }

    maxt = 0;
    qsort((void * )(p + 1), number_of_processes, sizeof(p[1]), cmp_burst_time);

    for (int i = 1; i <= number_of_processes; i++) {
        p[i].waiting_time = curr_time - p[i].arrival_time;
        if (p[i].waiting_time < 0) p[i].waiting_time = 0;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        if (curr_time < p[i].arrival_time) {
            curr_time = p[i].arrival_time + p[i].burst_time;
            for (int j = p[i].arrival_time; j < curr_time; j++) {
                strcpy(time[j], p[i].process_id);
            }
        } else {
            curr_time = curr_time + p[i].burst_time;
            for (int j = 1; j <= p[i].burst_time; j++) {
                strcpy(time[curr_time - j], p[i].process_id);
            }
        }
        sum_wt += p[i].waiting_time;
        sum_tt += p[i].turnaround_time;
    }

    maxt = curr_time;
    print_table(p, number_of_processes);
    avg_wt = 1.0l * sum_wt / number_of_processes;
    avg_tt = 1.0l * sum_tt / number_of_processes;

    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n\n\n", avg_tt);
    print_gantt_chart(p, number_of_processes);

}

void sjf_p() {
    int number_of_processes;
    process p[100];
    int turnaround_time, waiting_time;
    int curr_time = 0;
    double avg_wt, avg_tt;
    int sum_wt = 0, sum_tt = 0;

    printf("Number of Processes: ");
    scanf("%d", & number_of_processes);

    for (int i = 1; i <= number_of_processes; i++) {
        printf("Process ID: ");
        scanf("%s", p[i].process_id);
        printf("arrival Time: ");
        scanf("%d", & p[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", & p[i].burst_time);
        p[i].active = 1;
        p[i].remaining_time = p[i].burst_time;
    }

    int t = 0;
    while (1) {
        int best_remaining_time_pos = -1;
        int cnt_active = 0;
        for (int i = 1; i <= number_of_processes; i++) {
            if (p[i].active && p[i].arrival_time <= t && (best_remaining_time_pos == -1 || p[i].remaining_time < p[best_remaining_time_pos].remaining_time)) {
                best_remaining_time_pos = i;
            }
            if (p[i].active)
                cnt_active += 1;
        }

        if (!cnt_active) {
            maxt = t;
            break;
        }
        if (best_remaining_time_pos == -1) {
            char s[5] = "  ";
            strcpy(time[t], s);
            t += 1;
            continue;
        }
        if (p[best_remaining_time_pos].active && p[best_remaining_time_pos].arrival_time <= t) {
            p[best_remaining_time_pos].remaining_time -= 1;
            strcpy(time[t], p[best_remaining_time_pos].process_id);
            if (!p[best_remaining_time_pos].remaining_time) {
                p[best_remaining_time_pos].active = 0;
                p[best_remaining_time_pos].turnaround_time = t + 1 - p[best_remaining_time_pos].arrival_time;
                p[best_remaining_time_pos].waiting_time = p[best_remaining_time_pos].turnaround_time - p[best_remaining_time_pos].burst_time;
                sum_wt += p[best_remaining_time_pos].waiting_time;
                sum_tt += p[best_remaining_time_pos].turnaround_time;
            }
        }

        t += 1;
    }

    print_table(p, number_of_processes);
    avg_wt = 1.0l * sum_wt / number_of_processes;
    avg_tt = 1.0l * sum_tt / number_of_processes;
    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n\n\n", avg_tt);
    print_gantt_chart(p, number_of_processes);
}

int main() {
    int ch;
    char cont;
    
    do {
        for (int i = 0; i < 1000; i++) strcpy(time[i], "  ");
        printf("\t\tCPU SCHEDULING ALGORITHMS \n1. FCFS\n2. SJF\n3. EXIT\nEnter your option: ");
        scanf("%d", & ch);
        switch (ch) {
        case 1:
            fcfs();
            break;
        case 2:
            printf("SJF CPU SCHEDULER\n");
            int ch1;
            printf("MENU\n1.Non Pre-emptive SJF\n2.Pre-emptive SJF\nEnter choice: ");
            scanf("%d", & ch1);
            if (ch1 == 1) {
                sjf_np();
            } else if (ch1 == 2) {
                sjf_p();
            } else {
                printf("Invalid choice\n");
            }
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice\n");
        }
        printf("Want to Continue (Y/N): ");
        scanf(" %c", & cont);
        printf("\n");
    } while (cont == 'y' || cont == 'Y');
}