
#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int completion_time;
};

void input_processes(struct Process p[], int n)
{
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;

        printf("Enter the arrival time and burst time for process %d: ",
               p[i].pid);

        if (scanf("%d %d", &p[i].arrival_time,
                  &p[i].burst_time) != 2 ||
            p[i].arrival_time < 0 || p[i].burst_time <= 0) {
            printf("Invalid input. Arrival time must be non-negative "
                   "and burst time must be positive.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void copy_processes(struct Process dest[],
                    const struct Process src[], int n)
{
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
        dest[i].turnaround_time = 0;
        dest[i].waiting_time = 0;
        dest[i].response_time = 0;
        dest[i].completion_time = 0;
    }
}

void sort_by_arrival(struct Process p[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time ||
                (p[j].arrival_time == p[j + 1].arrival_time &&
                 p[j].pid > p[j + 1].pid)) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void calculate_times(struct Process *p, int current_time)
{
    p->completion_time = current_time + p->burst_time;
    p->turnaround_time = p->completion_time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->burst_time;
    p->response_time = p->waiting_time;
}

void print_gantt_chart(struct Process p[], int n, const char *name)
{
    int current_time = 0;

    printf("\n=== %s ===\n", name);
    printf("Gantt Chart:\n");

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            printf("| Idle ");
            current_time = p[i].arrival_time;
        }

        printf("| P%d ", p[i].pid);
        current_time += p[i].burst_time;
    }
    printf("|\n");

    current_time = 0;
    printf("%d", current_time);

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time;
            printf("      %d", current_time);
        }

        current_time += p[i].burst_time;
        printf("      %d", current_time);
    }
    printf("\n");
}

void print_results(struct Process p[], int n)
{
    double total_waiting = 0;
    double total_turnaround = 0;
    double total_response = 0;

    printf("\nPID\tAT\tBT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].waiting_time,
               p[i].turnaround_time,
               p[i].response_time);

        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;
        total_response += p[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Response Time: %.2f\n", total_response / n);
}

void fcfs(struct Process p[], int n)
{
    sort_by_arrival(p, n);

    int current_time = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time;
        }

        calculate_times(&p[i], current_time);
        current_time = p[i].completion_time;
    }

    print_gantt_chart(p, n, "First Come First Served (FCFS)");
    print_results(p, n);
}

void sjf(struct Process p[], int n)
{
    int current_time = 0;

    for (int completed = 0; completed < n; completed++) {
        int shortest = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].completion_time != 0 ||
                p[i].arrival_time > current_time) {
                continue;
            }

            if (shortest == -1 ||
                p[i].burst_time < p[shortest].burst_time ||
                (p[i].burst_time == p[shortest].burst_time &&
                 p[i].arrival_time < p[shortest].arrival_time) ||
                (p[i].burst_time == p[shortest].burst_time &&
                 p[i].arrival_time == p[shortest].arrival_time &&
                 p[i].pid < p[shortest].pid)) {
                shortest = i;
            }
        }

        if (shortest == -1) {
            int next_arrival = -1;

            for (int i = 0; i < n; i++) {
                if (p[i].completion_time == 0 &&
                    (next_arrival == -1 ||
                     p[i].arrival_time < next_arrival)) {
                    next_arrival = p[i].arrival_time;
                }
            }

            current_time = next_arrival;
            completed--;
            continue;
        }

        calculate_times(&p[shortest],
                        current_time);
        current_time = p[shortest].completion_time;
    }

    struct Process ordered[n];

    for (int i = 0; i < n; i++) {
        ordered[i] = p[i];
    }

    /* Reorder completed processes by their start time. */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int start_j = ordered[j].completion_time
                        - ordered[j].burst_time;
            int start_next = ordered[j + 1].completion_time
                           - ordered[j + 1].burst_time;

            if (start_j > start_next) {
                struct Process temp = ordered[j];
                ordered[j] = ordered[j + 1];
                ordered[j + 1] = temp;
            }
        }
    }

    print_gantt_chart(ordered, n, "Shortest Job First (SJF)");
    print_results(p, n);
}

int main(void)
{
    int n;

    printf("Enter the number of processes: ");

    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return EXIT_FAILURE;
    }

    struct Process original[n];
    struct Process fcfs_processes[n];
    struct Process sjf_processes[n];

    input_processes(original, n);

    copy_processes(fcfs_processes, original, n);
    copy_processes(sjf_processes, original, n);

    fcfs(fcfs_processes, n);
    sjf(sjf_processes, n);

    return 0;
}




