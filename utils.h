#pragma once
#include <stdlib.h>
#include <time.h> 
#include "process.h"

#define MEMORY_SIZE 50
#define MAX_ATTEMPTS 50
#define MAX_FREE_TABLE_SIZE 50

typedef struct {
    int start_address;
    int size;
} FreeEntry; //creates a free block

typedef struct {
    int capacity;
    FreeEntry freeEntries[MAX_FREE_TABLE_SIZE];
} FreeTable; //stores a list of all the free blocks

int generateRandomAddress();
int isMemoryAvailable(int memory[], Process process, int address);
void allocateMemory(int memory[], int address, int size);
void allocateProcessRandomly(int memory[], Process process, int process_idx);
void printMemory(int memory[]);
void initializeFreeTable(FreeTable* freeTable);
void addToFreeTable(int memory[], FreeTable* freeTable);
void printFreeTable(FreeTable* freeTable);

