#include "memory_alloc_algo.h"


void printSpaceAllocatedToProcess(int memory[], Process process, int address){
    printf("Process %d of size %d allocated at address %d\n", process.pid, process.memory_required, address);
}

void implementBestFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses, int isCompact){
    int min_fragmentation;
    FreeEntry bestFitBlock = {-1, -1};

    while (*numProcesses > 0) { // Outer loop continues until all processes are allocated
        while (freeTable->capacity > 0){ //check if the free table is not empty
            min_fragmentation = MEMORY_SIZE + 1;
            int length = freeTable->capacity;
            int i = 0;
            while (i < length){ //iterate through the free table entries
                if (freeTable->freeEntries[i].size >= process.memory_required){ //check if the block is free and if it's size is greater than the process memory requirement
                    int fragmentation = freeTable->freeEntries[i].size - process.memory_required;
                    if (fragmentation < min_fragmentation){ //check if the fragmentation is less than the current minimum fragmentation
                        min_fragmentation = fragmentation;
                        bestFitBlock = freeTable->freeEntries[i];
                    }
                }
                i++;
            }
            
            if (bestFitBlock.start_address != -1 && bestFitBlock.size != -1){ //check if a block was found
                allocateMemory(memory, bestFitBlock.start_address, process.memory_required);
                printSpaceAllocatedToProcess(memory, process, bestFitBlock.start_address);
                addToProccessAddrTable(addrTable, process.pid, bestFitBlock.start_address);

                for (int i = 0; i < length; i++){ //update the free table to remove the allocated space
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
                fragmentationStats(&bf_fragmentation, freeTable);
                return;
            
            }
            else { //if no block was found, deallocate memory and print the current state of the system
                deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
                increaseNumDeallocations(&bf_num_deallocations);
                if (isCompact == 1){
                    compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
                }
            }    
        }
        // If freeTable is empty but not all processes have been allocated, force a deallocation
        if (*numProcesses > 0) {
            deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
            increaseNumDeallocations(&bf_num_deallocations);
            if (isCompact == 1){
                compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
            }
        }
    }
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
}

void implementWorstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses, int isCompact){
    int max_fragmentation;
    FreeEntry worstFitBlock = {-1, -1};

    while (*numProcesses > 0){ // Outer loop continues until all processes are allocated
        // printf("Free Table Capacity: %d\n", freeTable->capacity}
        while (freeTable->capacity > 0){ //check if the free table is not empty
            max_fragmentation = -1;
            int length = freeTable->capacity;
            int i = 0;
            while (i < length){ //iterate through the free table entries
                if (freeTable->freeEntries[i].size >= process.memory_required){ //check if the block is free and if it's size is greater than the process memory requirement
                    int fragmentation = freeTable->freeEntries[i].size - process.memory_required;
                    if (fragmentation > max_fragmentation){
                        max_fragmentation = fragmentation;
                        worstFitBlock = freeTable->freeEntries[i];
                    }
                }
                i++;
            }
            
            if (worstFitBlock.start_address != -1 && worstFitBlock.size != -1){ //check if a block was found
                allocateMemory(memory, worstFitBlock.start_address, process.memory_required);
                printSpaceAllocatedToProcess(memory, process, worstFitBlock.start_address);
                addToProccessAddrTable(addrTable, process.pid, worstFitBlock.start_address);

                for (int i = 0; i < length; i++){ //update the free table to remove the allocated space
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
                fragmentationStats(&wf_fragmentation, freeTable);
                return;
            
            }
            else { //if no block was found, deallocate memory and print the current state of the system
                deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
                increaseNumDeallocations(&wf_num_deallocations);
                if (isCompact == 1){
                    compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
                }
            }    
        }
        // If freeTable is empty but not all processes have been allocated, force a deallocation
        if (*numProcesses > 0) {
            deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
            increaseNumDeallocations(&wf_num_deallocations);
            if (isCompact == 1){
                compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
            }
        }
    
    }
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
}

void implementFirstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses, int isCompact){
    FreeEntry firstFitBlock = {-1, -1};

    while (*numProcesses > 0){ // Outer loop continues until all processes are allocated
        while(freeTable->capacity > 0){//check if the free table is not empty
            int i = 0;
            int length = freeTable->capacity;

            while (i < length){//iterate through the free table entries
                if (freeTable->freeEntries[i].size >= process.memory_required){//check if it's a free entry and if it's size is greater than the process memory requirement
                    firstFitBlock = freeTable->freeEntries[i];
                    break;
                }
                i++;
            }

            if (firstFitBlock.start_address != -1 && firstFitBlock.size != -1){// check if a block (firstFitBlock) was found 
                allocateMemory(memory, firstFitBlock.start_address, process.memory_required);
                printSpaceAllocatedToProcess(memory, process, firstFitBlock.start_address);
                addToProccessAddrTable(addrTable, process.pid, firstFitBlock.start_address);

                for(int i = 0; i < length; i++){//update the free table to remove the allocated space
                    if (freeTable->freeEntries[i].start_address == firstFitBlock.start_address){
                        if (firstFitBlock.size == process.memory_required){
                            // allocate memory, eliminate the entire free block
                        shiftFreeTableEntries(freeTable, i);
                        }
                        else{
                            freeTable->freeEntries[i].start_address = firstFitBlock.start_address + process.memory_required;
                            freeTable->freeEntries[i].size = firstFitBlock.size - process.memory_required;
                        }
                    }
                }
                fragmentationStats(&ff_fragmentation, freeTable);
                return;

            }
            else {//if no block was found, deallocate memory and print the current state of the system
                deallocateMemory(memory, addrTable, freeTable, process_arr, numProcesses);
                increaseNumDeallocations(&ff_num_deallocations);
                if (isCompact == 1){
                    compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
                }
            }
        }

         // If freeTable is empty but not all processes have been allocated, force a deallocation
        if (*numProcesses > 0) {
            deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
            increaseNumDeallocations(&ff_num_deallocations);
            if (isCompact == 1){
                compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
            }
        }
    }
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
   
}

void implementNextFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, FreeEntry* lastAllocatedBlock, Process process_arr [], int* numProcesses, int isCompact){
    FreeEntry nextFitBlock = {-1, -1};
    FreeEntry firstFreeBlock; //stores the address of the first block in the free table after the last allocated block

    while (*numProcesses > 0){ // Outer loop continues until all processes are allocated
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
                printSpaceAllocatedToProcess(memory, process, nextFitBlock.start_address);
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
                fragmentationStats(&nf_fragmentation, freeTable);
                return;
        
            }
            else {
                deallocateMemory(memory, addrTable, freeTable, process_arr, numProcesses);
                increaseNumDeallocations(&nf_num_deallocations);
                if (isCompact == 1){
                    compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
                }
            }
            
        }

        // If freeTable is empty but not all processes have been allocated, force a deallocation
        if (*numProcesses > 0) {
            deallocateMemory(memory, addrTable,freeTable, process_arr, numProcesses);
            increaseNumDeallocations(&nf_num_deallocations);
            if (isCompact == 1){
                compactMemory(memory, freeTable, addrTable, process_arr, *numProcesses);
            }
        }
    }
    printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", process.pid, process.memory_required);
    
}