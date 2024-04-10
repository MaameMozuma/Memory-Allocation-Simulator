#pragma once
#include "utils.h"

void allocateProcessSpace(int memory[], Process process, int address);
void implementBestFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process);
void implementWorstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process);
void implementFirstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process);
void implementNextFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, FreeEntry* lastAllocatedBlock);