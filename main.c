#include <stdlib.h>
#include <stdio.h>
#include "memory_allocation_algorithms.h"

int main(){
    memoryBlock* head = (struct memoryBlock*)malloc(sizeof(struct memoryBlock));
    int processIDs[5] = {1, 2, 3, 4, 5};
    int processSizes[5] = {4, 2, 2, 3, 8};
    initializeMemory(head);
    for (int i = 0; i < 5; i++){
        implementWorstFit(head, processIDs[i], processSizes[i]);
    }
    printMemory(head);

    return 0;
}