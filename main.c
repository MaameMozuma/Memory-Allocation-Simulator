#include "memory_alloc_algo.h"


int main(){
    int num_processes;
    int memory[MEMORY_SIZE] = {0}; // Example memory array (0 for free, 1 for allocated)
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

    for (int i = 0; i < num_processes; i++) { //retrieving the memory required for the number of processes entered
        int size = -1;
        printf("Enter memory required for process %d (separated by space): ", i + 1);
        scanf("%d", &size);
        createProcess(i+1, size);
    }


    for (int i = 0; i < num_of_processes/2; i++){ //allocating half the processes randomly
        allocateProcessRandomly(memory, processes_in_memory[i], i);
    }


    printf("Memory After a Process has been allocated randomly: \n");
    printMemory(memory);


    printAllProcesses();
    printf("\n");

    addToFreeTable(memory, &FreeTable); //creating the free blocks and putting it in the free table

    printFreeTable(&FreeTable);

    //using next fit for the next few processes
    int proc_idx;
    for (proc_idx = num_of_processes/2; proc_idx < num_of_processes; proc_idx++){
        // implementBestFit(memory, &FreeTable, processes_in_memory[proc_idx]); 
        // implementWorstFit(memory, &FreeTable, processes_in_memory[proc_idx]); 
        // implementFirstFit(memory, &FreeTable, processes_in_memory[proc_idx]); 
        implementNextFit(memory, &FreeTable, processes_in_memory[proc_idx], &lastAllocatedBlock);
    }

    printf("Memory After a Process has been allocated using the allocation function: \n");
    printMemory(memory);
    printf("\n");

    printAllProcesses();
    printf("\n");

    printf("Free table after allocation function implementation: \n");
    printFreeTable(&FreeTable);

    return 0;
}