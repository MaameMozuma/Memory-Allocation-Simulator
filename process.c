#include "process.h"


int num_of_processes = 0; //initialising number of processes currently in the `processes_in_memory` array

/**
 * The function `getProcessID` forks a child process, creates a process struct, and returns its process ID.
 * @return A pointer to a Process struct is being returned.
 */
Process *getProcessID(){
    Process *process = (Process *)malloc(sizeof(Process)); //creating a process struct
    process->pid = fork();

    if(process->pid == -1){ //if process failed to fork
        printf("Error creating process\n");
        exit(1);
    }
    else if(process->pid == 0){ //if process was successfully forked
        process->pid = getpid();
        exit(0);
    }
    else{
        return process;
    }
    
}

/**
 * The function `createProcess` creates a new process with the given process ID and memory requirements.
 * @param process_id The process ID of the new process
 * @param memory_required The memory requirements of the new process
 * @param index The index of the new process in the `processes_in_memory` array
 */
void createProcess(int process_id, int memory_required, int index){
    processes_in_memory[index].pid = process_id;
    processes_in_memory[index].memory_required = memory_required;
    num_of_processes += 1;
    printf("Process created with PID: %d and memory required: %d\n", processes_in_memory[index].pid, processes_in_memory[index].memory_required);
}


/**
 * The function `printAllProcesses` prints information about each process in memory, including process
 * ID, and memory requirements.
 */
void printAllProcesses(){
    for (int i = 0; i < num_of_processes; i++){
        printf("Process ID: %d\n", processes_in_memory[i].pid);
        printf("Memory Required: %d\n", processes_in_memory[i].memory_required);
        printf("\n");
    }
}

/**
 * The function `shiftProcessesInMemory` shifts processes in memory by overwriting the first
 * element with subsequent elements in the array to obey the FIFO rule.
 */
void shiftProcessesInMemory(){
    for (int i = 0; i < num_of_processes; i++){
        processes_in_memory[0] = processes_in_memory[i+1];
    }
    num_of_processes -= 1;
}

// int main(){
//     getProcessID(1, 100);  // Example usage
//     getProcessID(2, 200);  // Example usage
//     printAllProcesses();
//     return 0;
// }