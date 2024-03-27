#include <stdio.h>
#include "memory_allocation_algorithms.h"


void implementBestFit(memoryBlock* head, int processID, int processSize){
    memoryBlock* current = head;
    int minFragmentation = MEMORY_SIZE + 1;
    memoryBlock* bestFitBlock = NULL;
    while (current != NULL){
        if (current->status == 0 && current->size >= processSize){
            int fragmentation = current->size - processSize;
            if (fragmentation < minFragmentation){
                minFragmentation = fragmentation;
                bestFitBlock = current; 
            }
        }
        current = current->next;
    }
    if (bestFitBlock != NULL){
        int fragmentation = bestFitBlock->size - processSize;

        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (bestFitBlock->processIDs[i] == -1) { // Find an empty slot
                bestFitBlock->processIDs[i] = processID;
                bestFitBlock->processSizes[i] = processSize;
                break; // Exit loop after inserting process ID
            }
        }
        bestFitBlock->size = fragmentation;
        if (bestFitBlock->size == 0){
            bestFitBlock->status = 1;
        }

    }
    else {
        printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", processID, processSize);
    }
}


void implementWorstFit(memoryBlock* head, int processID, int processSize){
    memoryBlock* current = head;
    int maxFragmentation = -1;
    memoryBlock* worstFitBlock = NULL;
    while (current != NULL){
        if (current->status == 0 && current->size >= processSize){
            int fragmentation = current->size - processSize;
            if (fragmentation > maxFragmentation){
                maxFragmentation = fragmentation;
                worstFitBlock = current; 
            }
        }
        current = current->next;
    }
    if (worstFitBlock != NULL){
        int fragmentation = worstFitBlock->size - processSize;

        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (worstFitBlock->processIDs[i] == -1) { // Find an empty slot
                worstFitBlock->processIDs[i] = processID;
                worstFitBlock->processSizes[i] = processSize;
                break; // Exit loop after inserting process ID
            }
        }
        worstFitBlock->size = fragmentation;
        if (worstFitBlock->size == 0){
            worstFitBlock->status = 1;
        }

    }
    else {
        printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", processID, processSize);
    }
}

void implementFirstFit(memoryBlock* head, int processID, int processSize){
    memoryBlock* current = head;
    memoryBlock* firstFitBlock = NULL;

    //loop through the memory blocks to find the first block that fits the process
    while(current != NULL){
        if (current->status == 0 && current->size >= processSize){
            firstFitBlock = current;
            break;
        }
        current = current->next;
    }
    if (firstFitBlock != NULL){
        int fragmentation = firstFitBlock->size - processSize;

        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (firstFitBlock->processIDs[i] == -1) { // Find an empty slot
                firstFitBlock->processIDs[i] = processID;
                firstFitBlock->processSizes[i] = processSize;
                break; // Exit loop after inserting process ID
            }
        }
        // update block status to allocated
        firstFitBlock->size = fragmentation;
        if (firstFitBlock->size == 0){
            firstFitBlock->status = 1;
        }

    }
    else {
        printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", processID, processSize);
    }
}

void implementNextFit(memoryBlock* head, memoryBlock* lastBlock, int processID, int processSize){
    memoryBlock* current = lastBlock;
    memoryBlock* nextFitBlock = NULL;
    while(current != NULL){
        if (current->status == 0 && current->size >= processSize){
            nextFitBlock = current;
            break;
        }
        current = current->next;
        // wrap around to the head of the memory blocks if the end of the list is reached
        if (current == NULL){
            current = head;
        }
        // stop the loop if we return to the last allocated block
        else if (current == lastBlock){
            break;
        }
    }

    if (nextFitBlock != NULL){
        int fragmentation = nextFitBlock->size - processSize;

        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (nextFitBlock->processIDs[i] == -1) { // Find an empty slot
                nextFitBlock->processIDs[i] = processID;
                nextFitBlock->processSizes[i] = processSize;
                break; // Exit loop after inserting process ID
            }
        }
        nextFitBlock->size = fragmentation;
        if (nextFitBlock->size == 0){
            nextFitBlock->status = 1;
        }

    }
    else {
        printf("Unable to allocate process %d with size %d. No appropriate memory block found.\n", processID, processSize);
    }
}