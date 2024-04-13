#pragma once
#include "utils.h"

extern int best_fit_num_processes; //stores the number of process currently asking/have received memory for best fit
extern int worst_fit_num_processes; //stores the number of process currently asking/have received memory for worst fit
extern int first_fit_num_processes; //stores the number of process currently asking/have received memory for first fit
extern int next_fit_num_processes;


void allocateProcessSpace(int memory[], Process process, int address);
void implementBestFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses);
void implementWorstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses);
void implementFirstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses);
void implementNextFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, FreeEntry* lastAllocatedBlock, Process process_arr [], int* numProcesses);