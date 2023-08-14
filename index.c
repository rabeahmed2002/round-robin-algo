#include <stdio.h>

#define MAX_PROCESSES 5
#define TIME_QUANTA 2

struct Process {
    int id;
    int burstTime;
    int remainingTime;
};

void executeRoundRobin(struct Process processes[], int n) {
    int currentTime = 0;
    int completedProcesses = 0;
    
    while (completedProcesses < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].remainingTime > 0) {
                printf("Time %d: Executing Process %d\n", currentTime, processes[i].id);
                if (processes[i].remainingTime <= TIME_QUANTA) {
                    currentTime += processes[i].remainingTime;
                    processes[i].remainingTime = 0;
                    completedProcesses++;
                } else {
                    currentTime += TIME_QUANTA;
                    processes[i].remainingTime -= TIME_QUANTA;
                }
            }
        }
    }
}

int main() {
    struct Process processes[MAX_PROCESSES] = {
        {1, 6, 6},
        {2, 4, 4},
        {3, 8, 8},
        {4, 3, 3},
        {5, 5, 5}
    };
    
    int n = sizeof(processes) / sizeof(processes[0]);
    
    printf("Round-Robin Scheduling:\n");
    executeRoundRobin(processes, n);
    
    return 0;
}
