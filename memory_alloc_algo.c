#include "memory_alloc_algo.h"


/**
 * The function `allocateProcessSpace` assigns a memory address to a process and prints a message
 * indicating the allocation.
 * 
 * @param memory The `memory` parameter is an array that represents the memory space available for
 * allocating processes.
 * @param process The `process` parameter is an object of type `Process` which contains information
 * about a process, such as its process ID (`pid`) and the amount of memory it requires
 * (`memory_required`).
 * @param address The `address` parameter in the `allocateProcessSpace` function represents the memory
 * address where the process will be allocated in the `memory` array. This address is where the process
 * will start in the memory space.
 */
void allocateProcessSpace(int memory[], Process process, int address){
    processes_in_memory[process.pid - 1].start_address = address;
    printf("Process %d of size %d allocated at address %d\n", process.pid, process.memory_required, address);
}


/**
 * The function `implementBestFit` searches for the best fit memory block to allocate memory for a
 * process based on its memory requirements. It searches for the smallest free memory block that can accommodate the
 * process and minimizes fragmentation.
 * 
 * @param memory The `memory` parameter in the `implementBestFit` function is an array representing the
 * memory blocks available for allocation. It likely contains information about the status of each
 * memory block, such as whether it is free or allocated.
 * @param freeTable The `FreeTable` structure seems to represent a table of free memory blocks in the
 * system. It likely contains an array of `FreeEntry` structures, where each `FreeEntry` represents a
 * free memory block with a start address and size.
 * @param process The `process` structure is a process with start address and size
 */
void implementBestFit(int memory[], FreeTable* freeTable, Process process){
    int min_fragmentation = MEMORY_SIZE + 1;
    FreeEntry bestFitBlock = {-1, -1};
    int i = 0;
    int length = sizeof(freeTable->freeEntries) / sizeof(freeTable->freeEntries[0]);

    while (i < length){
        if (freeTable->freeEntries[i].start_address != -1 && freeTable->freeEntries[i].size >= process.memory_required){
            int fragmentation = freeTable->freeEntries[i].size - process.memory_required;
            if (fragmentation < min_fragmentation){
                min_fragmentation = fragmentation;
                bestFitBlock = freeTable->freeEntries[i];
            }
        }
        i++;
    }
    if (bestFitBlock.start_address != -1 && bestFitBlock.size != -1){
        allocateMemory(memory, bestFitBlock.start_address, process.memory_required);
        allocateProcessSpace(memory, process, bestFitBlock.start_address);

        for (int i = 0; i < length; i++){
            if (freeTable->freeEntries[i].start_address == bestFitBlock.start_address){
                if (bestFitBlock.size == process.memory_required){
                    freeTable->freeEntries[i].start_address = -1;
                    freeTable->freeEntries[i].size = -1;
                    freeTable->capacity -= 1;
                }
                else{
                    freeTable->freeEntries[i].start_address = bestFitBlock.start_address + process.memory_required;
                    freeTable->freeEntries[i].size = min_fragmentation;
                }
            }
        }
        
    }
    else {
            printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
    }
}


/**
 * The function `implementWorstFit` searches for the worst fit memory block to allocate memory for a
 * process in a free table. It searches for the largest free memory block that can accommodate the
 * process and minimizes fragmentation
 * 
 * @param memory The `memory` parameter is an array representing the memory blocks available for
 * allocation. It likely contains information about the status of each memory block, such as whether it
 * is free or allocated.
 * @param freeTable The `FreeTable` parameter in the `implementWorstFit` function represents a data
 * structure that keeps track of the free memory blocks available for allocation. It contains an array
 * of `FreeEntry` structures, where each `FreeEntry` structure represents a free memory block with its
 * start address and size
 * @param process a Process struct representing the process to be allocated.
 */
void implementWorstFit(int memory[], FreeTable* freeTable, Process process){
    int max_fragmentation = -1;
    FreeEntry worstFitBlock = {-1, -1};
    int i = 0;
    int length = sizeof(freeTable->freeEntries) / sizeof(freeTable->freeEntries[0]);

    while (i < length){
        if (freeTable->freeEntries[i].start_address != -1 && freeTable->freeEntries[i].size >= process.memory_required){
            int fragmentation = freeTable->freeEntries[i].size - process.memory_required;
            if (fragmentation > max_fragmentation){
                max_fragmentation = fragmentation;
                worstFitBlock = freeTable->freeEntries[i];
            }
        }
        i++;
    }
    if (worstFitBlock.start_address != -1 && worstFitBlock.size != -1){
        allocateMemory(memory, worstFitBlock.start_address, process.memory_required);
        allocateProcessSpace(memory, process, worstFitBlock.start_address);

        for (int i = 0; i < length; i++){
            if (freeTable->freeEntries[i].start_address == worstFitBlock.start_address){
                if (worstFitBlock.size == process.memory_required){
                    freeTable->freeEntries[i].start_address = -1;
                    freeTable->freeEntries[i].size = -1;
                    freeTable->capacity -= 1;
                }
                else{
                    freeTable->freeEntries[i].start_address = worstFitBlock.start_address + process.memory_required;
                    freeTable->freeEntries[i].size = max_fragmentation;
                }
            }
        }
        
    }
    else {
            printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
    }
}