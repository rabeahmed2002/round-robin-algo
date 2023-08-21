#include <iostream>
#include <queue>
#include <iomanip>
#include <vector>
#include <sstream>

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
    int start_time;
    int arrival_time;
};

// Round Robin scheduling function
void roundRobinScheduling(queue<PCB>& ready_queue, int time_quantum, vector<int>& execution_times, vector<int>& turnaround_times, vector<int>& response_times) {
    int current_time = 0;

    cout << setw(15) << "Current Time" << setw(5) << "ID" << setw(14) << "PC" << setw(14) << "IR" << setw(16) << "Resume" << setw(18) << "Burst Time"
         << setw(20) << "Remaining Time" << setw(12) << "State" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------" << endl;

    while (!ready_queue.empty()) {
        PCB current_process = ready_queue.front();
        ready_queue.pop();

        // Simulate process execution for the time quantum
        int execution_time = min(time_quantum, current_process.remaining_time);
        current_process.remaining_time -= execution_time;

        string current_instruction = "Process" + to_string(current_process.process_id) + "[" + to_string(execution_times[current_process.process_id - 1]) + "]";
        string next_instruction = "Process" + to_string(current_process.process_id) + "[" + to_string(execution_times[current_process.process_id - 1] + 1) + "]";

        if (current_process.state == "Waiting") {
            response_times[current_process.process_id - 1] = current_time;
            current_process.state = "Running";
            current_process.ir = current_process.pc + 1;
            current_process.pc = current_process.ir;
            current_process.resume = current_process.pc + 1;
            if (current_process.start_time == -1) {
                current_process.start_time = current_time; // Set the start time
            }
        }

        if (current_process.remaining_time > 0) {
            ready_queue.push(current_process); // Re-queue the process if it's not completed
        } else {
            current_process.state = "Finished";
            current_process.ir = -1;
            current_process.resume = -1;
            turnaround_times[current_process.process_id - 1] = current_time - current_process.arrival_time;
        }

        current_time += execution_time;
        execution_times[current_process.process_id - 1] += execution_time;

        cout << setw(15) << current_time << setw(5) << current_process.process_id
             << setw(14) << current_instruction << setw(14) << next_instruction
             << setw(16) << "Process" << current_process.process_id << "[" << current_process.resume << "]"
             << setw(18) << current_process.burst_time 
             << setw(20) << current_process.remaining_time
             << setw(12) << current_process.state << endl;
    }
}

int main() {
    int num_processes;
    cout << "Enter the number of processes: ";
    cin >> num_processes;

    vector<PCB> processes;
    vector<int> execution_times(num_processes, 0);
    vector<int> turnaround_times(num_processes, 0);
    vector<int> response_times(num_processes, 0);

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
        process.start_time = 0; // Initialize start time to 0
        process.arrival_time = 0;

        processes.push_back(process);
    }

    int time_quantum;
    cout << "Enter time quantum: ";
    cin >> time_quantum;

    queue<PCB> ready_queue;
    int current_time = 0;

    // Set the same arrival time for all processes
    for (int i = 0; i < num_processes; ++i) {
        processes[i].arrival_time = 0; // Set the arrival time to 0 for all processes
    }
    for (int i = 0; i < num_processes; ++i) {
        processes[i].start_time = current_time; // Set the start time
        current_time += time_quantum; // Increment current time based on time quantum
        ready_queue.push(processes[i]);
    }

    roundRobinScheduling(ready_queue, time_quantum, execution_times, turnaround_times, response_times);

    cout << "\nProcess Metrics Table:\n";
    cout << setw(10) << "ProcessID" << setw(10) << "Start" << setw(10) << "Arrival" << setw(12) << "Execution"
         << setw(12) << "Turnaround" << setw(10) << "Response" << setw(10) << "Waiting" << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < num_processes; ++i) {
        int turnaround_time = turnaround_times[i];
        int response_time = response_times[i];
        int waiting_time = turnaround_time - execution_times[i];


        // cout << setw(10) << i + 1 << setw(10) << processes[i].start_time << setw(10) << processes[i].arrival_time
        //      << setw(10) << current_time << setw(12) << execution_times[i] << setw(12) << turnaround_time
        //      << setw(10) << response_time << setw(10) << waiting_time << endl;
        cout << setw(10) << i + 1 << setw(10) << processes[i].start_time << setw(10) << processes[i].arrival_time
             << setw(12) << execution_times[i] << setw(12) << turnaround_time
             << setw(10) << response_time << setw(10) << waiting_time << endl;     
    }

    return 0;
}
