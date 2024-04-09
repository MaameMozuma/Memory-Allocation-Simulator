#pragma once
#include <stdlib.h>
#include <time.h> 
#include "process.h"

#define MEMORY_SIZE 50
#define MAX_ATTEMPTS 50
#define MAX_FREE_TABLE_SIZE 50
#define MAX_PROCESS_ADDRESS_TABLE_SIZE 50

typedef struct {
    int start_address;
    int size;
} FreeEntry; //creates a free block

typedef struct {
    int capacity;
    FreeEntry freeEntries[MAX_FREE_TABLE_SIZE];
} FreeTable; //stores a list of all the free blocks

typedef struct {
    pid_t pid; 
    int base; 
} ProcessAddrEntry; //creates an entry for each process id 

typedef struct {
    int capacity;
    ProcessAddrEntry ProcessAddrEntries[MAX_PROCESS_ADDRESS_TABLE_SIZE];    
} ProcessAddrTable; //stores a list of the process ids and their start addresses in memory

int generateRandomAddress();
int generateRandomMemorySize();
int isMemoryAvailable(int memory[], Process process, int address);
void allocateMemory(int memory[], int address, int size);
void deallocateMemory(int memory[], ProcessAddrTable* addrTable, FreeTable* freeTable);
void allocateProcessRandomly(int memory[], Process process, ProcessAddrTable* addrTable, int process_idx);
void printMemory(int memory[]);
void initializeFreeTable(FreeTable* freeTable);
void addToFreeTable(int memory[], FreeTable* freeTable);
void shiftFreeTableEntries(FreeTable* freeTable, int index);
void printFreeTable(FreeTable* freeTable);
void initializeProcessAddrTable(ProcessAddrTable* addrTable);
void addToProccessAddrTable(ProcessAddrTable* addrTable, pid_t pid, int startAddress);
void shiftProcessAddrEntries(ProcessAddrTable* addrTable);
void printProcessAddrTable(ProcessAddrTable* addrTable);