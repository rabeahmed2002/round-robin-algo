#include <iostream>
#include <queue>
#include <iomanip>

using namespace std;

// Process Control Block (PCB) structure
struct PCB {
    int process_id;
    int burst_time;
    int remaining_time;
    string state;
    int pc;
    int ir;
    int resume;
    int start_time;      // Start time of execution
    int arrival_time;    // Arrival time of the process
    int turnaround_time; // Turnaround time
    int response_time;   // Response time
};

// Round Robin scheduling function
void roundRobinScheduling(queue<PCB>& ready_queue, int time_quantum) {
    int current_time = 0;
    double total_turnaround_time = 0.0;
    double total_response_time = 0.0;
    int total_utilization_time = 0;

    cout << setw(10) << "Process ID" << setw(12) << "Burst Time" << setw(15) << "Remaining Time" << setw(10) << "State"
         << setw(15) << "PC" << setw(15) << "IR" << setw(15) << "Resume" << setw(15) << "Start Time" << setw(15) << "Arrival Time"
         << setw(20) << "Turnaround Time" << setw(15) << "Response Time" << setw(15) << "Utilization" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------" << endl;

    while (!ready_queue.empty()) {
        PCB current_process = ready_queue.front();
        ready_queue.pop();

        if (current_process.state == "Waiting") {
            current_process.start_time = current_time;
            current_process.response_time = current_time;
            current_process.arrival_time = current_time;
            current_process.state = "Running";
        }

        // Simulate process execution for the time quantum
        int execution_time = min(time_quantum, current_process.remaining_time);
        current_process.remaining_time -= execution_time;
        current_time += execution_time;

        if (current_process.remaining_time > 0) {
            current_process.ir = current_process.pc + 1;
            current_process.pc = current_process.ir;
            current_process.resume = current_process.pc + 1;
            ready_queue.push(current_process); // Re-queue the process if it's not completed
        } else {
            current_process.state = "Finished";
            current_process.ir = -1;
            current_process.resume = -1;
            current_process.turnaround_time = current_time - current_process.arrival_time;
            total_turnaround_time += current_process.turnaround_time;
            total_response_time += current_process.response_time;
            total_utilization_time += current_process.burst_time;
        }

        cout << setw(10) << current_process.process_id << setw(12) << current_process.burst_time << setw(15) << current_process.remaining_time
             << setw(10) << current_process.state << setw(15) << "Process" << current_process.process_id << "[" << current_process.pc << "]"
             << setw(15) << "Process" << current_process.process_id << "[" << current_process.ir << "]" 
             << setw(15) << "Process" << current_process.process_id << "[" << current_process.resume << "]"
             << setw(15) << current_process.start_time << setw(15) << current_process.arrival_time
             << setw(20) << current_process.turnaround_time << setw(15) << current_process.response_time
             << setw(15) << (static_cast<double>(current_process.burst_time) / current_process.turnaround_time) << endl;
    }

    double average_turnaround_time = total_turnaround_time / ready_queue.size();
    double average_response_time = total_response_time / ready_queue.size();
    double cpu_utilization = static_cast<double>(total_utilization_time) / current_time;

    cout << "\nAverage Turnaround Time: " << average_turnaround_time << endl;
    cout << "Average Response Time: " << average_response_time << endl;
    cout << "CPU Utilization: " << cpu_utilization * 100 << "%" << endl;
}

int main() {
    int num_processes;
    cout << "Enter the number of processes: ";
    cin >> num_processes;

    queue<PCB> ready_queue;
    for (int i = 0; i < num_processes; ++i) {
        PCB process;
        process.process_id = i + 1;

        cout << "Enter burst time for process " << process.process_id << ": ";
        cin >> process.burst_time;
        process.remaining_time = process.burst_time;
        process.state = "Waiting";
        process.pc = 0;
        process.ir = 1;
        process.resume = 1;

        ready_queue.push(process);
    }

    int time_quantum;
    cout << "Enter time quantum: ";
    cin >> time_quantum;

    roundRobinScheduling(ready_queue, time_quantum);

    return 0;
}
