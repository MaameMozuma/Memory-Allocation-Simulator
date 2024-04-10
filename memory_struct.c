#include <stdio.h>
#include <stdlib.h>
#include "memory_struct.h"

void initializeMemory(memoryBlock* firstNode) {
    int blockNum = 0;
    int startAddress = 0;
    memoryBlock* current = firstNode;

    while (current != NULL && blockNum < NUM_BLOCKS) {
        current->blockNum = blockNum;
        current->startAddress = startAddress;
        current->size = BLOCK_SIZE;
        current->status = 0; // 0 for free, 1 for allocated 

        // Initialize processIDs and processSizes arrays
        for (int i = 0; i < BLOCK_SIZE; i++) {
            current->processIDs[i] = -1; // -1 for unallocated process
            current->processSizes[i] = 0; // Initial sizes are 0
        }

        startAddress += BLOCK_SIZE;
        blockNum++;

        // Allocate memory for the next node only if more blocks are needed
        if (blockNum < NUM_BLOCKS) {
            current->next = (memoryBlock*)malloc(sizeof(memoryBlock));
            current = current->next;
        } else {
            // If it's the last block, set its next pointer to NULL
            current->next = NULL;
        }
    }
}


void printMemory(memoryBlock* head){
    while (head != NULL){
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (head->processIDs[i] != -1){
                printf("Block Number: %d, Start Address: %d, Size Left: %d, Process ID: %d, Process Size: %d, Status: %d\n",
                head->blockNum, head->startAddress, head->size, head->processIDs[i], head->processSizes[i], head->status);
            }
        }
        head = head -> next;
    }
}
