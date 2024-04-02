#pragma once
#include "utils.h"

void allocateProcessSpace(int memory[], Process process, int address);
void implementBestFit(int memory[], FreeTable* freeTable, Process process);
void implementWorstFit(int memory[], FreeTable* freeTable, Process process);
void implementFirstFit(int memory[], FreeTable* freeTable, Process process);
void implementNextFit(int memory[], FreeTable* freeTable, Process process, FreeEntry* lastAllocatedBlock);