#include "utils.h"
int bf_fragmentation = -1; 
int wf_fragmentation = -1; 
int ff_fragmentation = -1; 
int nf_fragmentation = -1; 

int bf_num_deallocations = -1;
int wf_num_deallocations = -1;
int ff_num_deallocations = -1;
int nf_num_deallocations = -1;

int generateRandomAddress(){
    // Define the range for the random number
    int lowerBound = 0;
    int upperBound = MEMORY_SIZE;

    // Generate a random number within the specified range
    int randomNumber = (rand() % (upperBound - lowerBound + 1)) + lowerBound;

    return randomNumber;
}

int generateRandomMemorySize(int lowerBound, int upperBound){
    if (upperBound > MEMORY_SIZE/2) {
        upperBound = MEMORY_SIZE/3;
    }
    int randomNumber = (rand() % (upperBound - lowerBound + 1)) + lowerBound; // Generate random number between 1 and less than half of MEMORY_SIZE
    return randomNumber;
}

int isMemoryAvailable(int memory[], Process process, int address){
    // Check if the address plus memory required exceeds memory size
    if (address + process.memory_required > MEMORY_SIZE) {
        return 0; // Memory space not available
    }
    
    // Check each memory location within the required range
    for (int i = address; i < address + process.memory_required; i++) {
        if (memory[i] == 1) {
            return 0; 
        }
    }
    return 1; // Memory space available
}


void allocateMemory(int memory[], int address, int size){
    for (int i = address; i < address + size; i++) {
        memory[i] = 1; // Mark memory as allocated
    }
}

void deallocateMemory(int memory[], ProcessAddrTable* addrTable, FreeTable* freeTable, Process process_arr [], int* numProcesses){
    int memory_address = addrTable->ProcessAddrEntries[0].base; //get the base address of the oldest process
    int memory_size = process_arr[0].memory_required;
    int process_id = process_arr[0].pid;

    //eliminate first process from address table and processes in memory
    shiftProcessAddrEntries(addrTable);
    shiftProcessesInMemory(process_arr, numProcesses, 0); 

    freeTable->capacity = 0;
    for (int i = memory_address; i < memory_address + memory_size; i++){
        memory[i] = 0;
    }
    addToFreeTable(memory, freeTable);

    printf("Memory deallocated for process %d\n", process_id);
}

int allocateProcessRandomly(int memory[], Process process, ProcessAddrTable* addrTable, int process_idx){
    int address;
    int num_attemps = 0;
    int allocated = 0; //0 == False, 1 == True

    // Generate random addresses until memory space is available
    while (allocated == 0 && num_attemps < MAX_ATTEMPTS) {
        if (process.memory_required > MEMORY_SIZE){
            printf("Process %d of size %d exceeds memory size\n", process.pid, process.memory_required);
            exit(1);
        }
        address = generateRandomAddress();
        // check available memory for allocation
        if (isMemoryAvailable(memory, process, address) == 1) {
            allocateMemory(memory, address, process.memory_required);
            addToProccessAddrTable(addrTable, process.pid, address);
            allocated = 1; //process has been allocated
            printf("Process %d of size %d allocated at address %d\n", process.pid, process.memory_required, address);
        }
        num_attemps ++;
    }
    // If max attempts reached without finding a suitable memory space, notify the user
    if (allocated == 0) {
        printf("Unable to allocate memory for process %d after %d attempts\n", process.pid, num_attemps);
    }
    return allocated;
}

// void printMemory(int memory[]){
//     printf("Physical Memory: \n");
//     printf("+-----------------+--------+\n");
//     printf("| Memory Address  | Status |\n");
//     printf("+-----------------+--------+\n");
//     for (int i = 0; i < MEMORY_SIZE; i++) {
//         printf("|%-7s0x%-7x |%-7d |\n","", i, memory[i]);
//     }
//     printf("+-----------------+--------+\n");
// }

void printMemory(int memory[]){
    printf("Physical Memory: \n");
    printf("+-----------------+--------+\n");
    printf("| Memory Address  | Status |\n");
    printf("+-----------------+--------+\n");
    int start = 0;
    int status = memory[0];
    for (int i = 1; i < MEMORY_SIZE; i++) {
        if (memory[i] != status) {
            printf("|0x%-3x to 0x%-5x |%-7s |\n", start, i-1, status ? "Used" : "Free");
            start = i;
            status = memory[i];
        }
    }
    printf("|0x%-3x to 0x%-5x |%-7s |\n", start, MEMORY_SIZE-1, status ? "Used" : "Free");
    printf("+-----------------+--------+\n");
}

void initializeFreeTable(FreeTable* freeTable){
    freeTable->capacity = 0;

    for (int i = 0; i < MEMORY_SIZE; i++){
        freeTable->freeEntries[i].start_address = -1;
        freeTable->freeEntries[i].size = -1;
    }
}

void addToFreeTable(int memory[], FreeTable* freeTable){
    int startAddress = -1;
    int endAddress = -1;
    
    int i = 0;
    while (i < MEMORY_SIZE){
        int count = 0;
        int j = i;

        // updating the size of the free table 
        while (memory[j] != 1 && j < MEMORY_SIZE){
            count ++;
            j ++;
        }
        endAddress = j;
        if (count > 0){
            startAddress = endAddress - count;
            freeTable->freeEntries[freeTable->capacity].start_address = startAddress;
            freeTable->freeEntries[freeTable->capacity].size = count;
            freeTable->capacity += 1;
        }
        i = endAddress + 1;
    }

}

void shiftFreeTableEntries(FreeTable* freeTable, int index){
    // moving the entries in the free table to the left
    for (int i = index; i < freeTable->capacity; i++){
        freeTable->freeEntries[i] = freeTable->freeEntries[i+1];
    }
    freeTable->capacity -= 1;    
}

void printFreeTable(FreeTable* freeTable) {
    printf("Free Table Contents:\n");
    printf("Capacity: %d\n", freeTable->capacity);
    printf("Free Entries:\n");
    printf("+-------+-------+------+\n");
    printf("| Entry | Start | Size |\n");
    printf("+-------+-------+------+\n");
    for (int i = 0; i < freeTable->capacity; i++) {
        printf("|%-7d|%-7d|%-6d|\n", i + 1, freeTable->freeEntries[i].start_address, freeTable->freeEntries[i].size);
    }
    printf("+-------+-------+------+\n");
}

void initializeProcessAddrTable(ProcessAddrTable* addrTable){
    addrTable->capacity = 0;

    for (int i = 0; i < MEMORY_SIZE; i++){
        addrTable->ProcessAddrEntries[i].pid = -1;
        addrTable->ProcessAddrEntries[i].base = -1;
    }

}

void addToProccessAddrTable(ProcessAddrTable* addrTable, pid_t pid, int startAddress){
    addrTable->ProcessAddrEntries[addrTable->capacity].pid = pid;
    addrTable->ProcessAddrEntries[addrTable->capacity].base = startAddress;
    addrTable->capacity += 1;
}

void shiftProcessAddrEntries(ProcessAddrTable* addrTable){
    // moving the entries in the process address table to the left
    for (int i = 0; i < addrTable->capacity; i++){
        addrTable->ProcessAddrEntries[i] = addrTable->ProcessAddrEntries[i+1];
    }
    addrTable->capacity -= 1;
}

void printProcessAddrTable(ProcessAddrTable* addrTable){
    printf("Process Address Table Contents:\n");
    printf("Capacity: %d\n", addrTable->capacity);
    printf("Process Address Table Entries:\n");
    printf("+-------+-------+---------------+\n");
    printf("| Entry |  PID  | Start Address |\n");
    printf("+-------+-------+---------------+\n");
    for (int i = 0; i < addrTable->capacity; i++) {
        if (addrTable->ProcessAddrEntries[i].pid != -1){
            printf("|%-7d|%-7d|%-15d|\n", i + 1, addrTable->ProcessAddrEntries[i].pid, addrTable->ProcessAddrEntries[i].base);
        }
    } 
    printf("+-------+-------+---------------+\n");  
}

void compactMemory(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process_arr [], int numProcesses){
    int writeIndex = 0;

    ProcessAddrEntry* sortedArray = malloc(addrTable->capacity * sizeof(ProcessAddrEntry));
    // create copy of the process address table
    memcpy(sortedArray, addrTable->ProcessAddrEntries, addrTable->capacity * sizeof(ProcessAddrEntry));
    qsort(sortedArray, addrTable->capacity, sizeof(ProcessAddrEntry), compare); //sort in ascending order of base address
    
    // search for unallocated space to write to in memory
    for (int readIndex = 0; readIndex < MEMORY_SIZE; readIndex++) {
        if (memory[readIndex] == 1) {
            memory[writeIndex] = 1;
            if (writeIndex != readIndex) {
                memory[readIndex] = 0;
            }
            writeIndex++;
        }
    }

    int currAddress = 0;
    int processIndex = 0;
    for (int i = 0; i < numProcesses; i++) {
        int processIndex = findIndex(process_arr, numProcesses, sortedArray[i].pid);
        if (processIndex != -1) {
            int mem_required = process_arr[processIndex].memory_required;
            process_arr[processIndex].pid = sortedArray[i].pid;
            addrTable->ProcessAddrEntries[processIndex].base = currAddress;
            currAddress += mem_required;
        }
    }
    freeTable->capacity = 0;
    addToFreeTable(memory, freeTable);
    free(sortedArray);
}

int compare(const void* a, const void* b){
    ProcessAddrEntry* entryA = (ProcessAddrEntry*) a;
    ProcessAddrEntry* entryB = (ProcessAddrEntry*) b;

    return entryA->base - entryB->base;
}

int findIndex(Process process_arr [], int numProcesses, int value) {
    for (int i = 0; i < numProcesses; i++) {
        if (process_arr[i].pid == value) {
            return i;  // Return the index if the value is found
        }
    }
    return -1;  // Return -1 if the value is not found
}

void fragmentationStats(int *value, FreeTable* freeTable){
    if (value == NULL || freeTable == NULL) {
        printf("Error: Passed pointer is NULL.\n");
        return;
    }

    int fragmentation = freeTable->capacity;
    if(fragmentation > *value){
        *value = fragmentation;
    }
}

void increaseNumDeallocations(int *value){
    if (*value == -1){
        *value = 0;
    }
    *value += 1;
}

int *initialiseMemory(int memorySize){
    int* memory = malloc(memorySize * sizeof(int));
    for(int i = 0; i < memorySize; i++){
        memory[i] = 0;
    }
    return memory;
}
