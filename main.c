#include "memory_alloc_algo.h"
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void readNumProcesses(int *num_processes, int max_num_processes) {
    printf("Enter the number of processes (up to %d): ", max_num_processes);
    scanf("%d", num_processes);

    // Validate the number of processes
    if (*num_processes < 1 || *num_processes > max_num_processes) {
        printf("Invalid number of processes. Exiting...\n");
        exit(1); // or return an error code if necessary
    }
}

void initialize(FreeTable *Freetable, ProcessAddrTable *AddrTable) {
    initializeFreeTable(Freetable);
    initializeProcessAddrTable(AddrTable);
}

void createParentProcess(Process *parent_process, int lowerbound, int upperbound) {
    pid_t pid = getpid();
    int memory_required = generateRandomMemorySize(lowerbound, upperbound); // or any other appropriate range
    createProcess(pid, memory_required, 0);
}

void createAdditionalProcesses(Process* process, Process process_arr[], int num_processes, int lowerbound, int upperbound) {
    for (int i = 1; i < num_processes; i++) {
        process = getProcessID();
        int memory_required = generateRandomMemorySize(lowerbound, upperbound); // or any other appropriate range
        createProcess(process->pid, memory_required, i);
        upperbound += 2;
    }
}

void handle_sigsev(int signal) {
    if (signal == SIGUSR1) {
        printf("Segmentation fault occurred\n");
        exit(1);
    }
}

void handle_sigint(int signal) {
    if (signal == SIGINT) {
        printf("Interrupt signal received\n");
        exit(1);
    }
}

int main(){
    signal(SIGSEGV, handle_sigsev); // Handle segmentation fault signal
    signal(SIGINT, handle_sigint); // Handle interrupt signal
    int num_processes;
    int maxProcesses;
    int memory[MEMORY_SIZE] = {0}; // Memory array (0 for free, 1 for allocated)
    Process *process = malloc(sizeof(Process));
    FreeTable Freetable;
    FreeTable freeTableCopyBF;
    FreeTable freeTableCopyWF;
    FreeTable freeTableCopyFF;
    FreeTable freeTableCopyNF;
    ProcessAddrTable AddrTable;
    ProcessAddrTable AddrTableCopyBF;
    ProcessAddrTable AddrTableCopyWF;
    ProcessAddrTable AddrTableCopyFF;
    ProcessAddrTable AddrTableCopyNF;
    FreeEntry lastAllocatedBlock = {-1, -1};
    int best_fit_num_processes, worst_fit_num_processes, first_fit_num_processes, next_fit_num_processes;
    int isCompact;


    // int bfFragmentation, wfFragmentation, ffFragmentation, nfFragmentation;

    srand(time(NULL)); // Seed the random number generator

    printf("------Starting simulation------\n");
    printMemory(memory);

    // Prompt the user to enter a value for MAX_PROCESSES
    printf("Enter the maximum number of processes: ");
    scanf("%d", &maxProcesses);

    // Assign the user-input value to MAX_PROCESSES
    #undef MAX_PROCESSES
    #define MAX_PROCESSES maxProcesses

    readNumProcesses(&num_processes, maxProcesses); //reading the number of processes from the user

    // Prompt user for compaction
    printf("Enter 1 to simulate with compaction, 0 otherwise: ");
    scanf("%d", &isCompact);
    printf("\n");

    initialize(&Freetable, &AddrTable); //initialising the free table and the process address table

    createParentProcess(process, 1, 5); //creating the parent process

    createAdditionalProcesses(process, processes_in_memory, num_processes, 5, 15); //creating the additional processes

    printAllProcesses(processes_in_memory, num_of_processes); //printing all the processes in memory

    best_fit_num_processes = worst_fit_num_processes = first_fit_num_processes = next_fit_num_processes = num_of_processes;
    
    // Copy the processes_in_memory array for the various algorithms
    Process best_fit_processes[MAX_PROCESSES];
    Process worst_fit_processes[MAX_PROCESSES];
    Process first_fit_processes[MAX_PROCESSES];
    Process next_fit_processes[MAX_PROCESSES];

    memcpy(best_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));
    memcpy(worst_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));
    memcpy(first_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));
    memcpy(next_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));

    //allocate memory for the first half of the processes randomly
    for (int i = 0; i < num_of_processes/2; i++){
        allocateProcessRandomly(memory, processes_in_memory[i], &AddrTable, i);
    }

    addToFreeTable(memory, &Freetable); //creating the free blocks and putting it in the free table

    // Copy the memory array for the various algorithms
    int BFmemoryCopy[MEMORY_SIZE];
    int WFmemoryCopy[MEMORY_SIZE];
    int FFmemoryCopy[MEMORY_SIZE];
    int NFmemoryCopy[MEMORY_SIZE];

    memcpy(BFmemoryCopy, memory, MEMORY_SIZE * sizeof(int));
    memcpy(WFmemoryCopy, memory, MEMORY_SIZE * sizeof(int));
    memcpy(FFmemoryCopy, memory, MEMORY_SIZE * sizeof(int));
    memcpy(NFmemoryCopy, memory, MEMORY_SIZE * sizeof(int));

    // Copy the FreeTable for the various algorithms
    freeTableCopyBF.capacity = Freetable.capacity;
    freeTableCopyWF.capacity = Freetable.capacity;
    freeTableCopyFF.capacity = Freetable.capacity;
    freeTableCopyNF.capacity = Freetable.capacity;

    // Copy each FreeEntry
    for (int i = 0; i < Freetable.capacity; i++) {
        freeTableCopyBF.freeEntries[i] = Freetable.freeEntries[i];
        freeTableCopyWF.freeEntries[i] = Freetable.freeEntries[i];
        freeTableCopyFF.freeEntries[i] = Freetable.freeEntries[i];
        freeTableCopyNF.freeEntries[i] = Freetable.freeEntries[i];
    }

    // Copy the ProcessAddrTable for the various algorithms
    AddrTableCopyBF.capacity = AddrTable.capacity;
    AddrTableCopyWF.capacity = AddrTable.capacity;
    AddrTableCopyFF.capacity = AddrTable.capacity;
    AddrTableCopyNF.capacity = AddrTable.capacity;

    // Copy each ProcessAddrEntry
    for (int i = 0; i < AddrTable.capacity; i++) {
        AddrTableCopyBF.ProcessAddrEntries[i] = AddrTable.ProcessAddrEntries[i];
        AddrTableCopyWF.ProcessAddrEntries[i] = AddrTable.ProcessAddrEntries[i];
        AddrTableCopyFF.ProcessAddrEntries[i] = AddrTable.ProcessAddrEntries[i];
        AddrTableCopyNF.ProcessAddrEntries[i] = AddrTable.ProcessAddrEntries[i];
    }

    printf("Status after random allocations \n");
    printMemory(memory);
    printf("\n");
   

    printFreeTable(&Freetable);
    printf("\n");

    printProcessAddrTable(&AddrTable);


    // //Running algorithms
    printf("\n\n\n Testing Best Fit Algorithm \n");
    printf("-----------------------------\n");

    for (int proc_idx = num_of_processes/2; proc_idx < num_of_processes; proc_idx++){
        implementBestFit(BFmemoryCopy, &freeTableCopyBF, &AddrTableCopyBF, processes_in_memory[proc_idx], best_fit_processes, &best_fit_num_processes, isCompact); 
    }

    printf("\nStatus after using the best fit allocation algorithm \n");
    printMemory(BFmemoryCopy);
    printf("\n");

    printAllProcesses(best_fit_processes, best_fit_num_processes);
    printf("\n");

    printProcessAddrTable(&AddrTableCopyBF);
    printf("\n");

    printFreeTable(&freeTableCopyBF);

    
    printf("\n\n\n Testing Worst Fit Algorithm \n");
    printf("-----------------------------\n");

    for (int proc_idx = num_of_processes/2; proc_idx < num_of_processes;  proc_idx++){
        implementWorstFit(WFmemoryCopy, &freeTableCopyWF, &AddrTableCopyWF, processes_in_memory[proc_idx], worst_fit_processes, &worst_fit_num_processes, isCompact); 
    }

    printf("\nStatus after using the worst fit allocation algorithm \n");
    printMemory(WFmemoryCopy);
    printf("\n");

    printAllProcesses(worst_fit_processes, worst_fit_num_processes);
    printf("\n");

    printProcessAddrTable(&AddrTableCopyWF);
    printf("\n");

    printFreeTable(&freeTableCopyWF);

    
    printf("\n\n\n Testing First Fit Algorithm \n");
    printf("-----------------------------\n");
    
    for (int proc_idx = num_of_processes/2; proc_idx < num_of_processes;  proc_idx++){
        implementFirstFit(FFmemoryCopy, &freeTableCopyFF, &AddrTableCopyFF, processes_in_memory[proc_idx], first_fit_processes, &first_fit_num_processes, isCompact); 
    }

    printf("\nStatus after using the first fit allocation algorithm \n");
    printMemory(FFmemoryCopy);
    printf("\n");

    printAllProcesses(first_fit_processes, first_fit_num_processes);
    printf("\n");

    printProcessAddrTable(&AddrTableCopyFF);
    printf("\n");

    printFreeTable(&freeTableCopyFF);


    printf("\n\n\n Testing Next Fit Algorithm \n");
    printf("-----------------------------\n");

    for (int proc_idx = num_of_processes/2; proc_idx < num_of_processes;  proc_idx++){
        implementNextFit(NFmemoryCopy, &freeTableCopyNF, &AddrTableCopyNF, processes_in_memory[proc_idx], &lastAllocatedBlock, next_fit_processes, &next_fit_num_processes, isCompact); 
    }

    printf("\nStatus after using the next fit allocation algorithm \n");
    printMemory(NFmemoryCopy);
    printf("\n");

    printAllProcesses(next_fit_processes, next_fit_num_processes);
    printf("\n");

    printProcessAddrTable(&AddrTableCopyNF);
    printf("\n");

    printFreeTable(&freeTableCopyNF);

    printf("Best: %d, Worst: %d, First: %d, Next: %d\n", bf_fragmentation, wf_fragmentation, ff_fragmentation, nf_fragmentation);
    //printf("Worst %d\n", wf_fragmentation);

}