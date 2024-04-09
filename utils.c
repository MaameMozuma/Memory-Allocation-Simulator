#include "utils.h"


/**
 * The function generates a random address within a specified range. 
 * @return The function `generateRandomAddress` returns a randomly generated integer within the range
 * of 0 to `MEMORY_SIZE`, where `MEMORY_SIZE` is a constant or variable defined elsewhere in the code.
 */
int generateRandomAddress(){
    // Define the range for the random number
    int lowerBound = 0;
    int upperBound = MEMORY_SIZE;

    // Generate a random number within the specified range
    int randomNumber = (rand() % (upperBound - lowerBound + 1)) + lowerBound;

    return randomNumber;
}

/**
 * The function generates a random memory size within a specified range.
 * 
 * @return The function `generateRandomMemorySize` returns a random integer value between 1 and half of
 * the `MEMORY_SIZE` value.
 */
int generateRandomMemorySize(){
    int lowerBound = 1;
    int upperBound = MEMORY_SIZE/4;
    int randomNumber = (rand() % (upperBound - lowerBound + 1)) + lowerBound;
    return randomNumber;
}


/**
 * The function `isMemoryAvailable` checks if a range of memory locations is available for a process
 * based on its memory requirements and current memory allocation.
 * 
 * @param memory An array representing the memory locations, where each element indicates whether that
 * memory location is currently in use (1) or available (0).
 * @param process Process is a data structure that contains information about a process, such as its
 * memory requirements.
 * @param address The `address` parameter in the `isMemoryAvailable` function represents the starting
 * memory address where the `Process` object `process` is trying to allocate memory. The function
 * checks if the memory locations starting from this address and spanning the `memory_required` size of
 * the process are available in the physical memory`
 * 
 * @return The function `isMemoryAvailable` returns an integer value: 
 * - If memory space is available for the process at the specified address, it returns 1 (indicating
 * memory space is available).
 * - If memory space is not available (either due to exceeding memory size or memory locations already
 * being used), it returns 0 (indicating memory space is not available).
 */
int isMemoryAvailable(int memory[], Process process, int address){
    // Check if the address plus memory required exceeds memory size
    if (address + process.memory_required > MEMORY_SIZE) {
        return 0; // Memory space not available
    }
    
    // Check each memory location within the required range
    for (int i = address; i < address + process.memory_required; i++) {
        if (memory[i] == 1) {
            return 0; // Memory space not available
        }
    }
    return 1; // Memory space available
}


/**
 * The function `allocateMemory` marks a range of memory addresses as allocated in an array
 * representing memory.
 * 
 * @param memory The `memory` parameter is an array that represents the memory space where allocation
 * will take place. Each element in the array corresponds to a memory address, and the value at that
 * address indicates whether it is allocated or not.
 * @param address The `address` parameter in the `allocateMemory` function represents the starting
 * index in the `memory` array where memory allocation should begin.
 * @param size The `size` parameter in the `allocateMemory` function represents the number of memory
 * slots that need to be allocated starting from the specified `address`. It determines how many memory
 * slots will be marked as allocated in the `memory` array.
 */
void allocateMemory(int memory[], int address, int size){
    for (int i = address; i < address + size; i++) {
        memory[i] = 1; // Mark memory as allocated
    }
}


/**
 * The function `deallocateMemory` deallocates memory occupied by a process in a memory management
 * system.
 * 
 * @param memory The `memory` parameter is an array that represents the physical memory where processes
 * are allocated. I
 * @param addrTable The `addrTable` parameter is a pointer to a structure or array named
 * `ProcessAddrTable`. This structure contains information about the memory addresses of
 * processes currently in memory.
 * @param freeTable The `freeTable` parameter in the `deallocateMemory` function is a pointer to a
 * structure or object of type `FreeTable`. This structure contains information about the free
 * memory blocks available in the system, such as the starting address and size of each free block. 
 */
void deallocateMemory(int memory[], ProcessAddrTable* addrTable, FreeTable* freeTable){
    int memory_address = addrTable->ProcessAddrEntries[0].base;
    int memory_size = processes_in_memory[0].memory_required;
    shiftProcessAddrEntries(addrTable);
    shiftFreeTableEntries(freeTable, 0);
    shiftProcessesInMemory();
    freeTable->capacity = 0;
    for (int i = memory_address; i < memory_address + memory_size; i++){
        memory[i] = 0;
    }
    addToFreeTable(memory, freeTable); //come back here
}

/**
 * The function `allocateProcessRandomly` attempts to allocate memory for a process randomly within a
 * given memory space, with a limit on the number of attempts.
 * 
 * @param memory The `memory` parameter is an array representing the memory space where processes can
 * be allocated. Each element in the array corresponds to a memory block.
 * @param process The `process` parameter in the `allocateProcessRandomly` function represents the
 * process that needs to be allocated memory. It contains information such as the process ID (`pid`)
 * and the amount of memory required by the process (`memory_required`). This information is used to
 * determine if there is enough memory available
 * @param process_idx The `process_idx` parameter in the `allocateProcessRandomly` function represents
 * the index of the current process being allocated in the `processes_in_memory` array. This index is
 * used to track the position of the process in the array for further reference or updates.
 */
void allocateProcessRandomly(int memory[], Process process, ProcessAddrTable* addrTable, int process_idx){
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
        if (isMemoryAvailable(memory, process, address) == 1) { //if there is memory available for allocation
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
}


/**
 * The function printMemory prints the contents of a given array representing physical memory along
 * with their corresponding memory addresses in hexadecimal format.
 * 
 * @param memory The function takes an integer array `memory` as a parameter, which represents
 * the physical memory. The function iterates over the elements of the `memory` array and prints the
 * index along with the value.
 */
void printMemory(int memory[]){
    printf("Physical Memory: \n");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("[0x%x]:  %d \n", i, memory[i]);
    }
    printf("\n");
}


/**
 * The function `initializeFreeTable` initializes a FreeTable structure by setting its capacity to 0
 * and initializing each free entry with start address and size set to -1.
 * 
 * @param freeTable The `initializeFreeTable` function initializes a `FreeTable` structure by setting
 * its capacity to 0 and initializing each entry in the `freeEntries` array with start addresses and
 * sizes set to -1.
 */
void initializeFreeTable(FreeTable* freeTable){
    freeTable->capacity = 0;

    for (int i = 0; i < MEMORY_SIZE; i++){
        freeTable->freeEntries[i].start_address = -1;
        freeTable->freeEntries[i].size = -1;
    }
}


/**
 * The function `addToFreeTable` scans a memory array to identify free memory blocks and adds them to a
 * FreeTable structure.
 * 
 * @param memory The `memory` parameter is an array that represents the memory blocks. Each element in
 * the array corresponds to a memory block, and a value of 1 indicates that the memory block is in use,
 * while a value of 0 indicates that the memory block is free.
 * @param freeTable The `freeTable` parameter is a pointer to a struct named `FreeTable`. This struct
 * contains information about free memory blocks in a memory allocation system. The `FreeTable`
 * struct has a member named `freeEntries`, which is an array of structs representing
 * individual free memory blocks.
 */
void addToFreeTable(int memory[], FreeTable* freeTable){
    int startAddress = -1;
    int endAddress = -1;
    
    int i = 0;
    while (i < MEMORY_SIZE){
        int count = 0;
        int j = i;

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


/**
 * The function `shiftFreeTableEntries` shifts elements in a FreeTable array to the left by one
 * position and decreases the capacity by one.
 * 
 * @param freeTable FreeTable is a data structure that contains information about free memory blocks
 */
void shiftFreeTableEntries(FreeTable* freeTable, int index){
    for (int i = index; i < freeTable->capacity; i++){
        freeTable->freeEntries[i] = freeTable->freeEntries[i+1];
    }
    freeTable->capacity -= 1;    
}

/**
 * The function `printFreeTable` prints the contents of a FreeTable structure including capacity and
 * free entries.
 * 
 * @param freeTable FreeTable is a data structure that contains information about free memory blocks.
 * It has a capacity indicating the total number of free entries it currently holds, and an array of FreeEntry
 * structures called freeEntries. Each FreeEntry structure represents a free memory block with a start
 * address and size. 
 */
void printFreeTable(FreeTable* freeTable) {
    printf("Free Table Contents:\n");
    printf("Capacity: %d\n", freeTable->capacity);
    printf("Free Entries:\n");
    for (int i = 0; i < freeTable->capacity; i++) {
        printf("Entry %d:\n", i + 1);
        printf("Start Address: %d and Size: %d \n", freeTable->freeEntries[i].start_address, freeTable->freeEntries[i].size);
        printf("\n");
    }
}

/**
 * The function `initializeProcessAddrTable` initializes the `ProcessAddrTable` structure by setting its
 * capacity to 0 and initializing each entry in the `ProcessAddrEntries` array with default values (pid
 * = - 1 and the base = -1)
 * @param addrTable The `addrTable` parameter is a pointer to a `ProcessAddrTable` structure
 */
void initializeProcessAddrTable(ProcessAddrTable* addrTable){
    addrTable->capacity = 0;

    for (int i = 0; i < MEMORY_SIZE; i++){
        addrTable->ProcessAddrEntries[i].pid = -1;
        addrTable->ProcessAddrEntries[i].base = -1;
    }

}

/**
 * The function `addToProccessAddrTable` adds a new entry to a process address table with the given
 * process ID and memory base address.
 * @param addrTable The `addrTable` parameter is a pointer to a `ProcessAddrTable` struct, which
 * contains information about process address entries.
 * @param pid The `pid` parameter is the process ID (pid) of a process. It is a unique identifier
 * assigned to each process running on a system.
 * @param memory The `memory` parameter in the `addToProccessAddrTable` function represents the base
 * memory address associated with a specific process identified by its `pid`. 
 */
void addToProccessAddrTable(ProcessAddrTable* addrTable, pid_t pid, int startAddress){
    addrTable->ProcessAddrEntries[addrTable->capacity].pid = pid;
    addrTable->ProcessAddrEntries[addrTable->capacity].base = startAddress;
    addrTable->capacity += 1;
}

/**
 * The function `shiftProcessAddrEntries` shifts all entries in a `ProcessAddrTable` structure to the
 * left by one position and decrements the capacity by one.
 * @param addrTable The `addrTable` parameter is a pointer to a `ProcessAddrTable` struct, which
 * contains information about process address entries.
 */
void shiftProcessAddrEntries(ProcessAddrTable* addrTable){
    for (int i = 0; i < addrTable->capacity; i++){
        addrTable->ProcessAddrEntries[i] = addrTable->ProcessAddrEntries[i+1];
    }
    addrTable->capacity -= 1;
}

/**
 * The function `printProcessAddrTable` prints the contents of a process address table, including
 * process IDs and start addresses.
 * 
 * @param addrTable The function `printProcessAddrTable` is designed to print the contents of a
 * `ProcessAddrTable` structure. The structure contains information about process address table
 * entries, including the capacity and an array of `ProcessAddrEntry` elements.
 */
void printProcessAddrTable(ProcessAddrTable* addrTable){
    printf("Process Address Table Contents:\n");
    printf("Capacity: %d\n", addrTable->capacity);
    printf("Process Address Table Entries:\n");
    for (int i = 0; i < addrTable->capacity; i++) {
        if (addrTable->ProcessAddrEntries[i].pid != -1){
            printf("Entry %d:\n", i + 1);
            printf("Process Id: %d and Start Address: %d \n", addrTable->ProcessAddrEntries[i].pid, addrTable->ProcessAddrEntries[i].base);
            printf("\n");
        }
    }   
}

/**
 * The `compactMemory` function reorganizes memory by moving active processes to the beginning and
 * updating their base addresses in the process address table.
 * 
 * @param memory The `memory` parameter is an array representing the memory space. Each element in the
 * array corresponds to a memory block, and the value `1` indicates that the block is occupied by a
 * process, while `0` indicates that the block is free.
 * @param freeTable The `freeTable` parameter is a pointer to a `FreeTable` structure that contains free blocks
 * @param addrTable `addrTable` is a structure that contains information about the addresses of
 * processes in memory. It includes an array of `ProcessAddrEntry` structures, each representing a
 * process in memory with its base address and process ID. The `capacity` field indicates the total
 * number of entries that can be stored in the address table
 */
void compactMemory(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable){
    int writeIndex = 0;
    ProcessAddrEntry* sortedArray = malloc(addrTable->capacity * sizeof(ProcessAddrEntry));
    memcpy(sortedArray, addrTable->ProcessAddrEntries, addrTable->capacity * sizeof(ProcessAddrEntry));
    qsort(sortedArray, addrTable->capacity, sizeof(ProcessAddrEntry), compare);
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
    int index = 0;
    for (int i = 0; i < num_of_processes; i++){
        index = findIndex(sortedArray[i].pid);
        addrTable->ProcessAddrEntries[index].base = currAddress;
        currAddress += processes_in_memory[index].memory_required;
    }
    freeTable->capacity = 0;
    addToFreeTable(memory, freeTable);
    free(sortedArray);
}


/**
 * The function `compare` compares the base addresses of two `ProcessAddrEntry` structures.
 * 
 * @param a The `a` parameter is a pointer to a `ProcessAddrEntry` object, which is being cast from a
 * `void` pointer in the `compare` function.
 * @param b The parameter `b` in the `compare` function is a pointer to a `void` type. In this specific
 * implementation, it is being cast to a `ProcessAddrEntry*` type to compare two `ProcessAddrEntry`
 * structures based on their `base` member.
 * 
 * @return The `compare` function is returning the result of subtracting the `base` value of `entryB`
 * from the `base` value of `entryA`. This result will be an integer value representing the comparison
 * between the two `ProcessAddrEntry` structures.
 */
int compare(const void* a, const void* b){
    ProcessAddrEntry* entryA = (ProcessAddrEntry*) a;
    ProcessAddrEntry* entryB = (ProcessAddrEntry*) b;

    return entryA->base - entryB->base;
}

/**
 * The function `findIndex` searches for a specific value(pid) in an array of processes and returns the
 * index if found, or -1 if not found.
 * 
 * @param value The `value` parameter represents the process ID that you are searching for in the
 * `processes_in_memory` array. The function `findIndex` iterates through the array to find the index
 * of the element that matches the given `value`. If the `value` is found, the function returns
 * 
 * @return The function `findIndex` returns the index of the element with the specified value in the
 * `processes_in_memory` array if it is found. If the value is not found in the array, the function
 * returns -1.
 */
int findIndex(int value) {
    for (int i = 0; i < num_of_processes; i++) {
        if (processes_in_memory[i].pid == value) {
            return i;  // Return the index if the value is found
        }
    }
    return -1;  // Return -1 if the value is not found
}



// int main(){
//     int num_processes;
//     int memory[MEMORY_SIZE] = {0}; // Example memory array (0 for free, 1 for allocated)
//     FreeTable FreeTable;

//     srand(time(NULL)); // Seed the random number generator

//     initializeFreeTable(&FreeTable);

//     printf("Memory Initially: \n");
//     printMemory(memory);

//     printf("Enter the number of processes (up to %d): ", MAX_PROCESSES);
//     scanf("%d", &num_processes);

//     // Validate the number of processes
//     if (num_processes < 1 || num_processes > MAX_PROCESSES) {
//         printf("Invalid number of processes. Exiting...\n");
//         return 1;
//     }

//     for (int i = 0; i < num_processes; i++) {
//         int size = -1;
//         printf("Enter memory required for process %d (separated by space): ", i + 1);
//         scanf("%d", &size);
//         createProcess(i+1, size);
//     }


//     for (int i = 0; i < num_of_processes; i++){
//         allocateProcessRandomly(memory, processes_in_memory[i], i);
//     }


//     printf("Memory After a Process has been allocated: \n");
//     printMemory(memory);


//     printAllProcesses();
//     printf("\n");

//     addToFreeTable(memory, &FreeTable);

//     printFreeTable(&FreeTable);

//     return 0;
// }
