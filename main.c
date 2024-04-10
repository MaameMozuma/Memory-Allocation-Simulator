#include "memory_alloc_algo.h"
#include <unistd.h>


int main(){
    int num_processes;
    int memory[MEMORY_SIZE] = {0}; // Example memory array (0 for free, 1 for allocated)
    Process *process = malloc(sizeof(Process));
    FreeTable FreeTable;
    ProcessAddrTable ProcessAddrTable;
    FreeEntry lastAllocatedBlock = {-1, -1};

    srand(time(NULL)); // Seed the random number generator

    initializeFreeTable(&FreeTable); //initialising free table

    initializeProcessAddrTable(&ProcessAddrTable); //initialising process address table

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
    pid_t pid = getpid();
    int memory_required = generateRandomMemorySize();
    createProcess(pid, memory_required, 0);


    for (int i = 1; i < num_processes; i++) { //retrieving the memory required for the number of processes entered
        int memory_required = generateRandomMemorySize();
        process = getProcessID();
        createProcess(process->pid, memory_required, i);
    }


    for (int i = 0; i < num_of_processes/2; i++){
        allocateProcessRandomly(memory, processes_in_memory[i], &ProcessAddrTable, i);
    }

    // printf("Memory Before Compaction: \n");
    // printMemory(memory);
    // printf("\n");  

    // compactMemory(memory, &FreeTable, &ProcessAddrTable);

    // printf("Memory After Compaction: \n");
    // printMemory(memory);  

    // printAllProcesses(); 

    // printFreeTable(&FreeTable);

    // printProcessAddrTable(&ProcessAddrTable);

    printf("Memory After a Process has been allocated randomly: \n");
    printMemory(memory);

    printAllProcesses();
    printf("\n");

    addToFreeTable(memory, &FreeTable); //creating the free blocks and putting it in the free table
    printFreeTable(&FreeTable);

    printProcessAddrTable(&ProcessAddrTable);

    int proc_idx;
    for (proc_idx = num_of_processes/2; proc_idx < num_of_processes; proc_idx++){
        // implementBestFit(memory, &FreeTable, processes_in_memory[proc_idx]); 
        // implementWorstFit(memory, &FreeTable, processes_in_memory[proc_idx]); 
        implementWorstFit(memory, &FreeTable, &ProcessAddrTable, processes_in_memory[proc_idx]); 
        // implementNextFit(memory, &FreeTable, &ProcessAddrTable, processes_in_memory[proc_idx], &lastAllocatedBlock);
    }

    printf("Memory After a Process has been allocated using the allocation function: \n");
    printMemory(memory);

    printAllProcesses();
    printf("\n");

    printProcessAddrTable(&ProcessAddrTable);

    printFreeTable(&FreeTable);

}