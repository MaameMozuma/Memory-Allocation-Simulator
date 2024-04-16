#ifndef UTILS_H
#define UTILS_H


#include <stdlib.h>
#include <time.h> 
#include "process.h"
#include <string.h>

#define MEMORY_SIZE 500
#define MAX_ATTEMPTS 50
#define MAX_FREE_TABLE_SIZE 250
#define MAX_PROCESS_ADDRESS_TABLE_SIZE 100

extern int bf_fragmentation; //initialising the fragmentation for best fit
extern int wf_fragmentation; //initialising the fragmentation for worst fit
extern int ff_fragmentation; //initialising the fragmentation for first fit
extern int nf_fragmentation; //initialising the fragmentation for next fit

extern int bf_num_deallocations;
extern int wf_num_deallocations;
extern int ff_num_deallocations;
extern int nf_num_deallocations;

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
} ProcessAddrTable; //stores a list of the process ids and their start addresses in memory (as ProcessAddrEntry)


/**
 * The function generates a random address within a specified range. 
 * @return random address within the range of 0 to `MEMORY_SIZE`
 */
int generateRandomAddress();


/**
 * The function generates a random memory size within a specified range. 
 * @param lowerBound the lower bound of the memory size
 * @param upperBound the upper bound of the memory size [size ois MEMORY_SIZE/2 or MEMORY_SIZE/3]
 * @return random memory size within the range of `lowerBound` to `upperBound`
 */
int generateRandomMemorySize(int lowerBound, int upperBound);


/**
 * The function checks if the memory is available at a specified address for a given process.
 * @param memory the memory array
 * @param process the process accessing memory
 * @param address the address process is trying to allocate memory to
 * @return 1 if memory is available, 0 otherwise
 */
int isMemoryAvailable(int memory[], Process process, int address);


/**
 * The function allocates memory for a process at a specified address.
 * @param memory the memory array
 * @param address the address to allocate memory
 * @param size the size of the memory to allocate
 */
void allocateMemory(int memory[], int address, int size);


/**
 * The function deallocates memory for a process at a specified address.
 * @param memory the memory array
 * @param addrTable a pointer to the process address table
 * @param freeTable a pointer to 'FreeTable' struct tracking free blocks
 * @param process_arr the array of processes
 * @param numProcesses a pointer to the number of processes currently in memory
 */
void deallocateMemory(int memory[], ProcessAddrTable* addrTable, FreeTable* freeTable, Process process_arr [], int* numProcesses);


/**
 * The function allocates memory for a process at a random address.
 * @param memory the memory array
 * @param process the process being allocated memory
 * @param addrTable a pointer to the process address table
 * @param process_idx the index of the process being allocated in the array
 */
int allocateProcessRandomly(int memory[], Process process, ProcessAddrTable* addrTable, int process_idx);


/**
 * The function prints the representation of the physical memory with it's memory addresses
 * in hexadecimal form and its allocation status.
 * @param memory the memory array
 */
void printMemory(int memory[]);


/**
 * The function initializes a FreeTable with free entries of address and size -1.
 * @param freeTable pointer to the free table struct
 */
void initializeFreeTable(FreeTable* freeTable);


/**
 * The function adds a free block (entry) in physical memory to the free table.
 * @param memory the memory array to be scanned
 * @param freeTable pointer to the free table struct
 */
void addToFreeTable(int memory[], FreeTable* freeTable);


/**
 * The function shifts free entries, after a specified index, in the free table by one.
 * @param freeTable pointer to the free table struct
 * @param index starting index of the free block in the free table
 */
void shiftFreeTableEntries(FreeTable* freeTable, int index);


/**
 * The function prints the contents in the free table and its capacity.
 * @param freeTable pointer to the free table
 */
void printFreeTable(FreeTable* freeTable);


/**
 * The function initializes the process address table with `ProcessAddrEntries`, setting capacity
 * to 0, and setting each entry with a PID and base of -1.
 * @param addrTable pointer to a 'ProcessAddrTable' struct
 */
void initializeProcessAddrTable(ProcessAddrTable* addrTable);


/**
 * The function adds an entry to the process address table, updating its PID and starting address.
 * @param addrTable pointer the process address table
 * @param pid the process ID
 * @param startAddress the start address of the process in memory
 */
void addToProccessAddrTable(ProcessAddrTable* addrTable, pid_t pid, int startAddress);


/**
 * The function shifts the process address table entries by one and decrements the capacity by one.
 * @param addrTable pointer to the process address table
 */
void shiftProcessAddrEntries(ProcessAddrTable* addrTable);


/**
 * The function prints the contents of the process address table, including the PIDs and start addresses.
 * @param addrTable pointer to the process address table
 */
void printProcessAddrTable(ProcessAddrTable* addrTable);


/**
 * The function reorganises the memory by moving all processes to the beginning of the memory array,
 * updating all process base addresses and the free table accordingly.
 * @param memory the memory array
 * @param freeTable pointer to the free table
 * @param addrTable pointer to the process address table
 * @param process_arr the array of processes
 * @param numProcesses total number of processes in the system
 */
void compactMemory(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process_arr [], int numProcesses);


/**
 * The function compares base addresses of two `ProcessAddrEntry` structures.
 * @param a pointer to the first `ProcessAddrEntry` cast from `void*`
 * @param b pointer to the second `ProcessAddrEntry` cast from `void*`
 * @return integer value of subtraction of base addresses. 
 * Positive if a > b, negative if a < b, 0 if a == b
 */
int compare(const void* a, const void* b);


/**
 * The function searches for the index of a certain PID in the process array.
 * @param process_arr the array of processes
 * @param numProcesses the number of processes
 * @param value the process ID  being searched for
 * @return the index of the found process ID, -1 if not found
 */
int findIndex(Process process_arr [], int numProcesses, int value);

/**
 * The function tracks the computes the maximum fragmentation statistics for each memory allocation algorithm.
 * @param value pointer to the fragmentation value
 * @param freeTable pointer to the free table maintained by the algorithm
*/
void fragmentationStats(int *value, FreeTable* freeTable); //function to print the fragmentation statistics

void increaseNumDeallocations(int *value);

int *initialiseMemory(int memorySize);

#endif