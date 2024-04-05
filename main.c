#include "process.h"
#include "utils.h"
#include <unistd.h>


int main(){
    int num_processes;
    int memory[MEMORY_SIZE] = {0}; // Example memory array (0 for free, 1 for allocated)
    Process *process = malloc(sizeof(Process));
    FreeTable FreeTable;
    FreeEntry lastAllocatedBlock = {-1, -1};

    srand(time(NULL)); // Seed the random number generator

    initializeFreeTable(&FreeTable); //initialising free table

    printf("Memory Initially: \n");
    printMemory(memory);

    printf("Enter the number of processes (up to %d): ", MAX_PROCESSES);
    scanf("%d", &num_processes);

    // Validate the number of processes
    if (num_processes < 1 || num_processes > MAX_PROCESSES) {
        printf("Invalid number of processes. Exiting...\n");
        return 1;
    }

    // store parent process
    process->pid = getpid();
    process->memory_required = generateRandomMemorySize();
    printf("Process created with PID: %d and memory required: %d\n", process->pid, process->memory_required);
    processes_in_memory[0].pid = process->pid;
    processes_in_memory[0].memory_required = process->memory_required;


    for (int i = 1; i < num_processes; i++) { //retrieving the memory required for the number of processes entered
        int memory_required = generateRandomMemorySize();
        process = getProcessID();
        createProcess(process->pid, memory_required, i);
    }

    num_of_processes = num_processes;
    printAllProcesses();
}