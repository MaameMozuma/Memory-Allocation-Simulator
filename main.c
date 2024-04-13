#include "memory_alloc_algo.h"
#include <unistd.h>


int main(){
    int num_processes;
    int memory[MEMORY_SIZE] = {0}; // Example memory array (0 for free, 1 for allocated)
    Process *process = malloc(sizeof(Process));
    FreeTable Freetable;
    ProcessAddrTable AddrTable;
    FreeEntry lastAllocatedBlock = {-1, -1};

    srand(time(NULL)); // Seed the random number generator

    initializeFreeTable(&Freetable); //initialising free table

    initializeProcessAddrTable(&AddrTable); //initialising process address table

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
    int memory_required = generateRandomMemorySize(1, 5);
    createProcess(pid, memory_required, 0);

    //creating processes with random memory sizes
    int upperBound = 7;
    for (int i = 1; i < num_processes; i++) { //retrieving the memory required for the number of processes entered
        int memory_required = generateRandomMemorySize(2, upperBound);
        process = getProcessID();
        createProcess(process->pid, memory_required, i);
        upperBound += 2;
    }

    // Copy the processes in memory
    int best_fit_num_processes = num_of_processes;
    Process best_fit_processes[MAX_PROCESSES];
    memcpy(best_fit_processes, processes_in_memory, sizeof(processes_in_memory));

    // Copy the processes in memory
    int worst_fit_num_processes = num_of_processes;
    Process worst_fit_processes[MAX_PROCESSES];
    memcpy(worst_fit_processes, processes_in_memory, sizeof(processes_in_memory));

    // Copy the processes in memory
    int first_fit_num_processes = num_of_processes;
    Process first_fit_processes[MAX_PROCESSES];
    memcpy(first_fit_processes, processes_in_memory, sizeof(processes_in_memory));

    // Copy the processes in memory
    int next_fit_num_processes = num_of_processes;
    Process next_fit_processes[MAX_PROCESSES];
    memcpy(next_fit_processes, processes_in_memory, sizeof(processes_in_memory));


    //allocate memory for the first half of the processes randomly
    for (int i = 0; i < num_of_processes/2; i++){
        allocateProcessRandomly(memory, processes_in_memory[i], &AddrTable, i);
    }

    printf("Memory After a Process has been allocated randomly: \n");
    printMemory(memory);

    // Copy the memory array for the various algorithms
    int WFmemoryCopy[MEMORY_SIZE];
    memcpy(WFmemoryCopy, memory, MEMORY_SIZE * sizeof(int));

    // Copy the memory array for the various algorithms
    int FFmemoryCopy[MEMORY_SIZE];
    memcpy(FFmemoryCopy, memory, MEMORY_SIZE * sizeof(int));

    // Copy the memory array for the various algorithms
    int NFmemoryCopy[MEMORY_SIZE];
    memcpy(NFmemoryCopy, memory, MEMORY_SIZE * sizeof(int));

    addToFreeTable(memory, &Freetable); //creating the free blocks and putting it in the free table
    
    //creating a copy of the free table instance
    FreeTable freeTableCopyWF;
    freeTableCopyWF.capacity = Freetable.capacity;

    //creating a copy of the free table instance
    FreeTable freeTableCopyFF;
    freeTableCopyFF.capacity = Freetable.capacity;

    //creating a copy of the free table instance
    FreeTable freeTableCopyNF;
    freeTableCopyNF.capacity = Freetable.capacity;

    // Copy each FreeEntry
    for (int i = 0; i < Freetable.capacity; i++) {
        freeTableCopyWF.freeEntries[i] = Freetable.freeEntries[i];
        freeTableCopyFF.freeEntries[i] = Freetable.freeEntries[i];
        freeTableCopyNF.freeEntries[i] = Freetable.freeEntries[i];
    }

    //creating a copy of the process address table instance
    ProcessAddrTable AddrTableCopyWF;
    AddrTableCopyWF.capacity = AddrTable.capacity;

    //creating a copy of the process address table instance
    ProcessAddrTable AddrTableCopyFF;
    AddrTableCopyFF.capacity = AddrTable.capacity;

    //creating a copy of the process address table instance
    ProcessAddrTable AddrTableCopyNF;
    AddrTableCopyNF.capacity = AddrTable.capacity;

    // Copy each FreeEntry
    for (int i = 0; i < AddrTable.capacity; i++) {
        AddrTableCopyWF.ProcessAddrEntries[i] = AddrTable.ProcessAddrEntries[i];
        AddrTableCopyFF.ProcessAddrEntries[i] = AddrTable.ProcessAddrEntries[i];
        AddrTableCopyNF.ProcessAddrEntries[i] = AddrTable.ProcessAddrEntries[i];
    }
    
    printFreeTable(&Freetable);

    printProcessAddrTable(&AddrTable);


    // printf("\n\n\n Testing Best Fit Algorithm \n");

    // int proc_idx;
    // for (proc_idx = num_of_processes/2; proc_idx < num_of_processes; proc_idx++){
    //     implementBestFit(memory, &Freetable, &AddrTable, processes_in_memory[proc_idx], best_fit_processes, &best_fit_num_processes); 
    // }

    // printf("Memory After a Process has been allocated using the allocation function: \n");
    // printMemory(memory);

    // printAllProcesses(best_fit_processes, best_fit_num_processes);
    // printf("\n");

    // printProcessAddrTable(&AddrTable);

    // printFreeTable(&Freetable);



    // printf("\n\n\n Testing Worst Fit Algorithm \n");

    // printf("Memory After a Process has been allocated randomly: \n");
    // printMemory(WFmemoryCopy);
    
    // int process_idx;
    // for (process_idx = num_of_processes/2; process_idx < num_of_processes;  process_idx++){
    //     implementWorstFit(WFmemoryCopy, &freeTableCopyWF, &AddrTableCopyWF, processes_in_memory[process_idx], worst_fit_processes, &worst_fit_num_processes); 
    // }

    // printf("Memory After a Process has been allocated using the allocation function: \n");
    // printMemory(WFmemoryCopy);

    // printAllProcesses(worst_fit_processes, worst_fit_num_processes);
    // printf("\n");

    // printProcessAddrTable(&AddrTableCopyWF);

    // printFreeTable(&freeTableCopyWF);



    printf("\n\n\n Testing First Fit Algorithm \n");

    printf("Memory After a Process has been allocated randomly: \n");
    printMemory(FFmemoryCopy);
    
    int process_idx;
    for (process_idx = num_of_processes/2; process_idx < num_of_processes;  process_idx++){
        implementFirstFit(FFmemoryCopy, &freeTableCopyFF, &AddrTableCopyFF, processes_in_memory[process_idx], first_fit_processes, &first_fit_num_processes); 
    }

    printf("Memory After a Process has been allocated using the allocation function: \n");
    printMemory(FFmemoryCopy);

    printAllProcesses(first_fit_processes, first_fit_num_processes);
    printf("\n");

    printProcessAddrTable(&AddrTableCopyFF);

    printFreeTable(&freeTableCopyFF);

    
    
    
    printf("\n\n\n Testing Next Fit Algorithm \n");

    printf("Memory After a Process has been allocated randomly: \n");
    printMemory(WFmemoryCopy);
    

    for (process_idx = num_of_processes/2; process_idx < num_of_processes;  process_idx++){
        implementWorstFit(NFmemoryCopy, &freeTableCopyNF, &AddrTableCopyNF, processes_in_memory[process_idx], next_fit_processes, &next_fit_num_processes); 
    }

    printf("Memory After a Process has been allocated using the allocation function: \n");
    printMemory(NFmemoryCopy);

    printAllProcesses(next_fit_processes, next_fit_num_processes);
    printf("\n");

    printProcessAddrTable(&AddrTableCopyNF);

    printFreeTable(&freeTableCopyNF);



}