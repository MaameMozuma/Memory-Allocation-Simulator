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


/**
 * The function `implementFirstFit` searches for the first available memory block that fits a process
 * and allocates memory accordingly.
 * 
 * @param memory The `memory` parameter in the `implementFirstFit` function is an array representing
 * the memory blocks available for allocation. It is used to keep track of the memory status, whether
 * it is allocated to a process or free.
 * @param freeTable The `FreeTable` structure seems to contain an array of `FreeEntry` elements. Each
 * `FreeEntry` represents a block of free memory in the system, with a `start_address` and a `size`.
 * The `implementFirstFit` function is designed to allocate memory for a given process
 * @param process The `implementFirstFit` function you provided is designed to allocate memory for a
 * process using the First Fit algorithm. It searches for the first available memory block that can
 * accommodate the process based on its memory requirement.
 */
void implementFirstFit(int memory[], FreeTable* freeTable, Process process){
    FreeEntry firstFitBlock = {-1, -1};
    int i = 0;
    int length = sizeof(freeTable->freeEntries) / sizeof(freeTable->freeEntries[0]);

    // find the first block that fits the process
    while (i < length){
        //check if it's a free entry and if it's size is greater than the process memory requirement
        if (freeTable->freeEntries[i].start_address != -1 && freeTable->freeEntries[i].size >= process.memory_required){
            firstFitBlock = freeTable->freeEntries[i];
            break;
        }
        i++;
    }

    // check if a block (firstFitBlock) was found 
    if (firstFitBlock.start_address != -1 && firstFitBlock.size != -1){
        allocateMemory(memory, firstFitBlock.start_address, process.memory_required);
        allocateProcessSpace(memory, process, firstFitBlock.start_address);

        for(int i = 0; i < length; i++){
            if (freeTable->freeEntries[i].start_address == firstFitBlock.start_address){
                if (firstFitBlock.size == process.memory_required){
                    // allocate memory, eliminate the entire free block
                    freeTable->freeEntries[i].start_address = -1;
                    freeTable->freeEntries[i].size = -1;
                    freeTable->capacity -= 1;
                }
                else{
                    // update the free table to remove the space the process occupied only
                    freeTable->freeEntries[i].start_address = firstFitBlock.start_address + process.memory_required;
                    freeTable->freeEntries[i].size = firstFitBlock.size - process.memory_required;
                }
            }
        }
       }
       else {
            printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
         }
   
}


/**
 * The implementNextFit function allocates memory for a process using the Next Fit algorithm, updating
 * the free table accordingly.
 * 
 * @param memory The `memory` parameter is an array representing the memory space available for
 * allocation. It likely contains information about the memory blocks and their status (free or
 * allocated).
 * @param freeTable The `freeTable` parameter in the `implementNextFit` function represents a data
 * structure that holds information about the free memory blocks available for allocation. It likely
 * contains an array of `FreeEntry` structures, where each `FreeEntry` structure represents a free
 * memory block with a start address and size
 * @param process The `process` parameter in the `implementNextFit` function represents the process
 * that needs to be allocated memory. It contains information about the process, such as its process ID
 * (`pid`) and the amount of memory required by the process. The function attempts to find a suitable
 * memory block in the free
 * @param lastAllocatedBlock The `lastAllocatedBlock` parameter is a pointer to a `FreeEntry` struct
 * that represents the last allocated memory block. It contains the `start_address` and `size` of the
 * block that was last allocated to a process. This information is used in the `implementNextFit`
 * function
 */
void implementNextFit(int memory[], FreeTable* freeTable, Process process, FreeEntry* lastAllocatedBlock){
    FreeEntry nextFitBlock = {-1, -1};
    int i = 0; //start from the first entry in the free table if lastAllocatedBlock is at the end or not available
    int length = sizeof(freeTable->freeEntries) / sizeof(freeTable->freeEntries[0]);

    if(lastAllocatedBlock->start_address != -1 && lastAllocatedBlock->size != -1){
        // find the next block after the last allocated block that fits the process
        for (int j = 0; j < length; j++){
            if (freeTable->freeEntries[j].start_address == lastAllocatedBlock->start_address && freeTable->freeEntries[j].size == lastAllocatedBlock->size){
                i = (j + 1) % length; // move i to the next free block after the last allocated block
                break;
            }
        }
    }
    

    while(1){ //while true
        if (freeTable->freeEntries[i].start_address != -1 && freeTable->freeEntries[i].size >= process.memory_required){
            nextFitBlock = freeTable->freeEntries[i];
            // update the last allocated block to the current block
            lastAllocatedBlock->start_address = nextFitBlock.start_address;
            lastAllocatedBlock->size = nextFitBlock.size;
            break;
        }
        
        i = (i + 1) % length; // wrap around back to the beginning if the last block was at the end

        // end the loop if we are back to the last allocated block from the beginning
        if(freeTable->freeEntries[i].start_address == lastAllocatedBlock->start_address && freeTable->freeEntries[i].size == lastAllocatedBlock->size){
            break;
        }

    }

    // check if a block (NextFitBlock) was found 
    if (nextFitBlock.start_address != -1 && nextFitBlock.size != -1){
        allocateMemory(memory, nextFitBlock.start_address, process.memory_required);
        allocateProcessSpace(memory, process, nextFitBlock.start_address);

        printf("An allocation is complete\n");
        printMemory(memory);

        for (int i = 0; i < length; i++){
            if (freeTable->freeEntries[i].start_address == nextFitBlock.start_address){
                if (nextFitBlock.size == process.memory_required){
                    // allocate memory, eliminate the entire free block
                    freeTable->freeEntries[i].start_address = -1;
                    freeTable->freeEntries[i].size = -1;
                    freeTable->capacity -= 1;

                    break;

                }
                else{
                    //update free table to remove the processes' occupied space only
                    freeTable->freeEntries[i].start_address = nextFitBlock.start_address + process.memory_required;
                    freeTable->freeEntries[i].size = nextFitBlock.size - process.memory_required;

                    break;
                }
            }
        }

        printFreeTable(freeTable);
       
    }
    else {
        printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
    }
}