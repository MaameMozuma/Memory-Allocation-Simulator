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
    // processes_in_memory[process.pid - 1].start_address = address;
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
void implementBestFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses){
    int min_fragmentation;
    int isDeallocated = 0;
    FreeEntry bestFitBlock = {-1, -1};

    while (freeTable->capacity > 0){
        min_fragmentation = MEMORY_SIZE + 1;
        int length = freeTable->capacity;
        int i = 0;
        while (i < length){
            if (freeTable->freeEntries[i].size >= process.memory_required){
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
            addToProccessAddrTable(addrTable, process.pid, bestFitBlock.start_address);

            for (int i = 0; i < length; i++){
                if (freeTable->freeEntries[i].start_address == bestFitBlock.start_address){
                    if (bestFitBlock.size == process.memory_required){
                        shiftFreeTableEntries(freeTable, i);
                    }
                    else{
                        freeTable->freeEntries[i].start_address = bestFitBlock.start_address + process.memory_required;
                        freeTable->freeEntries[i].size = min_fragmentation;
                    }
                }
            }
            return;
        
        }
        else {
            deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
            printAllProcesses(process_arr, *numProcesses);
            printFreeTable(freeTable);
            printProcessAddrTable(addrTable);
        }    
    }
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
}



/**
 * The function `implementWorstFit` allocates memory for a process using the worst fit algorithm,
 * finding the largest available block that can accommodate the process.
 * 
 * @param memory The `memory` parameter is an array representing the memory blocks available for
 * allocation.
 * @param freeTable The `freeTable` parameter in the `implementWorstFit` function is a pointer to a
 * structure or object of type `FreeTable`.
 * @param addrTable The `addrTable` parameter in the `implementWorstFit` function is of type
 * `ProcessAddrTable*`, which is a pointer to a structure or object representing a table that maps
 * process IDs to their allocated memory addresses. This table is used to keep track of the memory
 * addresses allocated to each process
 * @param process Accepts a process to be allocated memory using the algorithm
 * 
 * @return void.
 */
void implementWorstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses){
    int max_fragmentation;
    FreeEntry worstFitBlock = {-1, -1};

    while (freeTable->capacity > 0){
        max_fragmentation = -1;
        int length = freeTable->capacity;
        int i = 0;
        while (i < length){
            if (freeTable->freeEntries[i].size >= process.memory_required){
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
            addToProccessAddrTable(addrTable, process.pid, worstFitBlock.start_address);

            for (int i = 0; i < length; i++){
                if (freeTable->freeEntries[i].start_address == worstFitBlock.start_address){
                    if (worstFitBlock.size == process.memory_required){
                        shiftFreeTableEntries(freeTable, i);
                    }
                    else{
                        freeTable->freeEntries[i].start_address = worstFitBlock.start_address + process.memory_required;
                        freeTable->freeEntries[i].size = max_fragmentation;
                    }
                }
            }
            return;
        
        }
        else {
            deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
            printAllProcesses(process_arr, *numProcesses);
            printFreeTable(freeTable);
            printProcessAddrTable(addrTable);
        }    
    }
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
}


void implementFirstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses){
    FreeEntry firstFitBlock = {-1, -1};

    while(freeTable->capacity > 0){
        int i = 0;
        int length = freeTable->capacity;

        // find the first block that fits the process
        while (i < length){
            //check if it's a free entry and if it's size is greater than the process memory requirement
            if (freeTable->freeEntries[i].size >= process.memory_required){
                firstFitBlock = freeTable->freeEntries[i];
                break;
            }
            i++;
        }

        // check if a block (firstFitBlock) was found 
        if (firstFitBlock.start_address != -1 && firstFitBlock.size != -1){
            allocateMemory(memory, firstFitBlock.start_address, process.memory_required);
            allocateProcessSpace(memory, process, firstFitBlock.start_address);
            addToProccessAddrTable(addrTable, process.pid, firstFitBlock.start_address);

            for(int i = 0; i < length; i++){
                if (freeTable->freeEntries[i].start_address == firstFitBlock.start_address){
                    if (firstFitBlock.size == process.memory_required){
                        // allocate memory, eliminate the entire free block
                       shiftFreeTableEntries(freeTable, i);
                    }
                    else{
                        // update the free table to remove the space the process occupied only
                        freeTable->freeEntries[i].start_address = firstFitBlock.start_address + process.memory_required;
                        freeTable->freeEntries[i].size = firstFitBlock.size - process.memory_required;
                    }
                }
            }
            return;

        }
        else {
            deallocateMemory(memory, addrTable, freeTable, process_arr, numProcesses);
            printAllProcesses(process_arr, *numProcesses);
            printFreeTable(freeTable);
            printProcessAddrTable(addrTable);
        }
    }
    
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
   
}


void implementNextFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, FreeEntry* lastAllocatedBlock, Process process_arr [], int* numProcesses){
    FreeEntry nextFitBlock = {-1, -1};
    FreeEntry firstFreeBlock; //stores the address of the first block in the free table after the last allocated block

    while(freeTable->capacity > 0){
        int i = 0; //start from the first entry in the free table if lastAllocatedBlock is at the end or not available
        int length = freeTable->capacity;

        if(lastAllocatedBlock->start_address != -1 && lastAllocatedBlock->size != -1){
        // find the next block after the last allocated block that fits the process
            for (int j = 0; j < length; j++){
                if (freeTable->freeEntries[j].start_address > (lastAllocatedBlock->start_address + lastAllocatedBlock->size)){
                    i = j; // move i to a free block in an address after the end of the last allocated block 
                    break;
                }
            }
        }
        // assign the first free block to where i is
        firstFreeBlock.start_address = freeTable->freeEntries[i].start_address;
        firstFreeBlock.size = freeTable->freeEntries[i].size;
    

        while(1){ //while true
            if (freeTable->freeEntries[i].start_address != -1 && freeTable->freeEntries[i].size >= process.memory_required){
                nextFitBlock = freeTable->freeEntries[i];
                // update the last allocated block to the current block
                lastAllocatedBlock->start_address = nextFitBlock.start_address;
                lastAllocatedBlock->size = process.memory_required;
                break;
            }
            i = (i + 1) % length; // wrap around back to the beginning if the last block was at the end

            // end the loop if we are back to the first free block from the beginning
            if(freeTable->freeEntries[i].start_address == firstFreeBlock.start_address && freeTable->freeEntries[i].size == firstFreeBlock.size){
                break;
            }

        }

        // check if a block (NextFitBlock) was found 
        if (nextFitBlock.start_address != -1 && nextFitBlock.size != -1){
            allocateMemory(memory, nextFitBlock.start_address, process.memory_required);
            allocateProcessSpace(memory, process, nextFitBlock.start_address);
            addToProccessAddrTable(addrTable, process.pid, nextFitBlock.start_address);

            for (int i = 0; i < length; i++){
                if (freeTable->freeEntries[i].start_address == nextFitBlock.start_address){
                    if (nextFitBlock.size == process.memory_required){
                        // allocate memory, eliminate the entire free block
                        shiftFreeTableEntries(freeTable, i);

                    }
                    else{
                        //update free table to remove the processes' occupied space only
                        freeTable->freeEntries[i].start_address = nextFitBlock.start_address + process.memory_required;
                        freeTable->freeEntries[i].size = nextFitBlock.size - process.memory_required;

                    }
                }
            }
            return;
    
        }
        else {
            deallocateMemory(memory, addrTable, freeTable, process_arr, numProcesses);
            printAllProcesses(process_arr, *numProcesses);
            printFreeTable(freeTable);
            printProcessAddrTable(addrTable);
        }
        
    }
    
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
    
}