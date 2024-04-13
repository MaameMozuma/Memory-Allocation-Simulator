#pragma once
#include "utils.h"

extern int best_fit_num_processes; //initialising number of processes currently in the `best_fit_processes` array
extern int worst_fit_num_processes; //initialising number of processes currently in the `worst_fit_processes` array
extern int first_fit_num_processes; //initialising number of processes currently in the `first_fit_processes` array
extern int next_fit_num_processes; //initialising number of processes currently in the `next_fit_processes` array

/**
 * Prints the space allocated to a process.
 * @param memory The array representing the memory.
 * @param process The Process object containing information about the process.
 * @param address The address at which the process is allocated.
 */
void printSpaceAllocatedToProcess(int memory[], Process process, int address);

/**
 * This function attempts to allocate memory for a process using the Best Fit algorithm.
 * It searches for the smallest free memory block that can accommodate the process.
 * If a suitable block is found, the memory is allocated and the process is added to the process address table.
 * If no suitable block is found, the function calls the deallocateMemory function to free up memory,
 * and then prints the current state of the processes, free table, and process address table.
 * @param memory The array representing the memory blocks.
 * @param freeTable The table containing information about the free memory blocks.
 * @param addrTable The table containing information about the allocated memory address for each process.
 * @param process The process to be allocated memory.
 * @param process_arr The array of all processes.
 * @param numProcesses The number of processes in the process array.
 */
void implementBestFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses);

/**
 * This function searches for the largest free memory block that can accommodate the process.
 * It then allocates the memory block to the process and updates the free table and process address table accordingly.
 * If no appropriate memory block is found, it deallocates memory and prints the current state of the system.
 * @param memory The array representing the memory blocks.
 * @param freeTable The free table containing information about the available free memory blocks.
 * @param addrTable The table containing information about the allocated memory address for each process.
 * @param process The process to be allocated.
 * @param process_arr The array of all processes.
 * @param numProcesses The number of processes in the system.
 */
void implementWorstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses);

/**
 * This function searches for the first available memory block that can accommodate the process.
 * If a suitable block is found, it allocates memory to the process and updates the necessary data structures.
 * If no suitable block is found, it deallocates memory, and prints the current state of the system
 * @param memory The memory array representing the system's memory.
 * @param freeTable The table of free memory blocks.
 * @param addrTable The table of allocated memory addresses for each process.
 * @param process The process to be allocated memory.
 * @param process_arr The array of all processes in the system.
 * @param numProcesses The number of processes in the system.
 */
void implementFirstFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, Process process_arr [], int* numProcesses);

/**
 * This function allocates memory for a process using the Next Fit algorithm.
 * It searches for the next available memory block after the last allocated block
 * that can accommodate the process. If no suitable block is found, it deallocates
 * memory for a processes and prints the current state of the system.
 * @param memory The array representing the memory blocks.
 * @param freeTable The table containing information about free memory blocks.
 * @param addrTable The table containing information about allocated memory addresses and the corresponding processes.
 * @param process The process to be allocated.
 * @param lastAllocatedBlock The last allocated memory block.
 * @param process_arr The array of all processes.
 * @param numProcesses The number of processes in the system.
 */
void implementNextFit(int memory[], FreeTable* freeTable, ProcessAddrTable* addrTable, Process process, FreeEntry* lastAllocatedBlock, Process process_arr [], int* numProcesses);