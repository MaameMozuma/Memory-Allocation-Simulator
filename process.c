#include "process.h"
int num_of_processes = 0;


/**
 * The function `createProcess` creates a new process with a given process ID and memory requirement,
 * as long as the maximum limit of processes has not been reached.
 * 
 * @param process_id The `process_id` parameter is an integer value that represents the unique
 * identifier of the process being created.
 * @param memory_required The `memory_required` parameter in the `createProcess` function represents
 * the amount of memory that the process needs to execute. This value indicates the memory resources
 * that the process will consume while running. 
 */
void createProcess(int process_id, int memory_required){
    if (num_of_processes < MAX_PROCESSES){
        processes_in_memory[num_of_processes].pid = process_id;
        processes_in_memory[num_of_processes].start_address = -1; //initially, memory has not been allocated
        processes_in_memory[num_of_processes].memory_required = memory_required;
        num_of_processes ++; //increase the number of processes
        printf("Process created with PID: %d and memory required: %d\n", process_id, memory_required);
    }
    else{
        printf("Cannot create process. Maximum limit reached.\n");
    }
}

/**
 * The function `printAllProcesses` prints information about each process in memory, including process
 * ID, memory addresses, and memory requirements.
 */
void printAllProcesses(){
    for (int i = 0; i < num_of_processes; i++){
        printf("Process ID: %d\n", processes_in_memory[i].pid);
        printf("Start Address: %d and End Address: %d \n", processes_in_memory[i].start_address, processes_in_memory[i].start_address + processes_in_memory[i].memory_required);
        printf("Memory Required: %d\n", processes_in_memory[i].memory_required);
        printf("\n");
    }
}

// int main(){
//     createProcess(1, 100);  // Example usage
//     createProcess(2, 200);  // Example usage
//     printAllProcesses();
//     return 0;
// }