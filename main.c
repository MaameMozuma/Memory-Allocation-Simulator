#include "memory_alloc_algo.h"
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

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
    printf("%d\n", num_processes);
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

void calcMemoryUsage(Process process_arr[], int num_processes, float *memory_usage) {
    for (int i = 0; i < num_processes; i++) {
        *memory_usage += process_arr[i].memory_required;
    }

    *memory_usage = (*memory_usage / MEMORY_SIZE) * 100;
}

void printStats(float bf_memory_usage, float wf_memory_usage, float ff_memory_usage, float nf_memory_usage,
                int bf_fragmentation, int wf_fragmentation, int ff_fragmentation, int nf_fragmentation,
                int bf_num_deallocations, int wf_num_deallocations, int ff_num_deallocations, int nf_num_deallocations
        ){
    printf("Statistics \n");
    printf("+-----------+--------------+--------------+-------------+\n");
    printf("| Algorithm | Memory Usage | Fragmentation | Deallocation |\n");
    printf("+-----------+--------------+--------------+-------------+\n");
    printf("| Best Fit  | %.2f%%       | %d            | %d           |\n", bf_memory_usage, bf_fragmentation, bf_num_deallocations);
    printf("| Worst Fit | %.2f%%       | %d            | %d           |\n", wf_memory_usage, wf_fragmentation, wf_num_deallocations);
    printf("| First Fit | %.2f%%       | %d            | %d           |\n", ff_memory_usage, ff_fragmentation, ff_num_deallocations);
    printf("| Next Fit  | %.2f%%       | %d            | %d           |\n", nf_memory_usage, nf_fragmentation, nf_num_deallocations);
    printf("+-----------+--------------+--------------+-------------+\n");
}


int main(){
    signal(SIGSEGV, handle_sigsev); // Handle segmentation fault signal
    signal(SIGINT, handle_sigint); // Handle interrupt signal
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
    pid_t* processes_unallocated;
    float bf_memory_usage = 0, wf_memory_usage = 0, ff_memory_usage = 0, nf_memory_usage = 0;


    // int bfFragmentation, wfFragmentation, ffFragmentation, nfFragmentation;

    srand(time(NULL)); // Seed the random number generator

    printf("------Starting simulation------\n");
    printMemory(memory);

    int num_processes;
    printf("Enter the number of processes (up to %d): ", MAX_PROCESSES);
    scanf("%d", &num_processes);
    printf("\n");

    // Validate the number of processes
    if (num_processes < 1 || num_processes > MAX_PROCESSES) {
        printf("Invalid number of processes. Exiting...\n");
        exit(1); // or return an error code if necessary
    }

    //readNumProcesses(&num_processes); //reading the number of processes from the user

    // Prompt user for compaction
    printf("Enter 1 to simulate with compaction, 0 otherwise: ");
    scanf("%d", &isCompact);
    printf("\n");

    printf("Number of processes: %d\n", num_processes); //printing the number of processes

    initialize(&Freetable, &AddrTable); //initialising the free table and the process address table

    createParentProcess(process, 1, 5); //creating the parent process

    createAdditionalProcesses(process, processes_in_memory, num_processes, 5, 15); //creating the additional processes

    printf("Number of processes: %d\n", num_processes); //printing the number of processes

    printAllProcesses(processes_in_memory, num_of_processes); //printing all the processes in memory

    best_fit_num_processes = worst_fit_num_processes = first_fit_num_processes = next_fit_num_processes = num_of_processes;
    
    // Copy the processes_in_memory array for the various algorithms
    Process best_fit_processes[MAX_PROCESSES];
    Process worst_fit_processes[MAX_PROCESSES];
    Process first_fit_processes[MAX_PROCESSES];
    Process next_fit_processes[MAX_PROCESSES];

    processes_unallocated = (pid_t*)malloc(num_processes * sizeof(pid_t));

    for (int i = 0; i < num_processes; i++) {
        processes_unallocated[i] = -1;
    }

    //allocate memory for the first half of the processes randomly
    for (int i = 0; i < num_of_processes/2; i++){
        int isAllocated = allocateProcessRandomly(memory, processes_in_memory[i], &AddrTable, i);
        if (isAllocated == 0){
            printf("Memory allocation failed for process %d\n", processes_in_memory[i].pid);
            processes_unallocated[i] = processes_in_memory[i].pid;
        }
    }

    addToFreeTable(memory, &Freetable); //creating the free blocks and putting it in the free table

    for (int i = 0; i < num_processes; i++) {
        if (processes_unallocated[i] != -1) {
            printf("Process %d was not allocated memory\n", processes_unallocated[i]);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        if (processes_unallocated[i] != -1) {
            int index = findIndex(processes_in_memory, num_of_processes, processes_unallocated[i]);
            shiftProcessesInMemory(processes_in_memory, &num_of_processes, index);
            printf("Process %d was not allocated memory\n", processes_unallocated[i]);
            best_fit_num_processes -= 1;
            worst_fit_num_processes -= 1;
            first_fit_num_processes -= 1;
            next_fit_num_processes -= 1;
        }
    }

    memcpy(best_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));
    memcpy(worst_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));
    memcpy(first_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));
    memcpy(next_fit_processes, processes_in_memory, num_of_processes * sizeof(Process));

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

    for (int proc_idx = AddrTable.capacity; proc_idx < num_of_processes; proc_idx++){
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

    for (int proc_idx = AddrTable.capacity; proc_idx < num_of_processes;  proc_idx++){
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
    
    for (int proc_idx = AddrTable.capacity; proc_idx < num_of_processes;  proc_idx++){
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

    for (int proc_idx = AddrTable.capacity; proc_idx < num_of_processes;  proc_idx++){
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


    calcMemoryUsage(best_fit_processes, best_fit_num_processes, &bf_memory_usage);
    calcMemoryUsage(worst_fit_processes, worst_fit_num_processes, &wf_memory_usage);
    calcMemoryUsage(first_fit_processes, first_fit_num_processes, &ff_memory_usage);
    calcMemoryUsage(next_fit_processes, next_fit_num_processes, &nf_memory_usage);

    printf("\n");
    printStats(bf_memory_usage, wf_memory_usage, ff_memory_usage, nf_memory_usage, bf_fragmentation, 
    wf_fragmentation, ff_fragmentation, nf_fragmentation, bf_num_deallocations, wf_num_deallocations, 
    ff_num_deallocations, nf_num_deallocations);

}