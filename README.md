# Process Scheduling Simulation (FCFS & SJF)

## Description
This project implements two non-preemptive process scheduling simulations in C under Linux:

- First Come First Served (FCFS)
- Shortest Job First (SJF)

The program calculates the waiting time, turnaround time, and response time of each process. It also displays a Gantt chart and calculates the average waiting, turnaround, and response times for each algorithm.

## Requirements
- Linux operating system
- GCC compiler
- Git

## How to Compile
Open a terminal in the project directory and run:

```bash
gcc -Wall -Wextra -std=c11 main.c -o scheduler
```

## How to Run

```bash
./scheduler
```

Enter the number of processes, followed by each process's arrival time and burst time when prompted.

## Example Input

```text
4
0 8
1 4
2 9
3 5
```

## Algorithms

**FCFS:** Processes execute in arrival order.

**SJF:** The available process with the shortest burst time executes first. Ties are resolved by earlier arrival time.

## Author
Nareh Sayadian# homework7.OS
