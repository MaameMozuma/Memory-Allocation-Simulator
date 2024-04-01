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
void allocateProcessRandomly(int memory[], Process process, int process_idx){
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
            processes_in_memory[process_idx].start_address = address; //update the process with its start address
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