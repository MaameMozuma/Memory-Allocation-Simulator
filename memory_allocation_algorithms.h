#include "memory_struct.h"

void implementBestFit(memoryBlock * firstNode, int processID, int processSize);
void implementWorstFit(memoryBlock * firstNode, int processID, int processSize);
void implementFirstFit(memoryBlock * firstNode, int processID, int processSize);
void implementNextFit(memoryBlock * firstNode, memoryBlock* lastAllocatedNode, int processID, int processSize);
