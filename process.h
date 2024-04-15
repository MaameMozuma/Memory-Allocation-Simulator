#ifndef PROCESS_H
#define PROCESS_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PROCESSES 0 //define the maximum number of processes that can be created

typedef struct {
    pid_t pid;  // Process ID
    int memory_required;  // Memory required by the process
} Process;

extern Process processes_in_memory[MAX_PROCESSES]; //stores all processes that have been successfully created
extern int num_of_processes; //stores the number of processes currently in the `processes_in_memory` array

/**
 * The function `getProcessID` forks a child process, creates a process struct, and returns its process ID.
 * @return A pointer to a Process struct is being returned.
 */
Process *getProcessID();

/**
 * Creates a new process with the given process ID and memory requirements.
 * @param process_id The ID of the process.
 * @param memory_required The amount of memory required by the process.
 * @param index The index in the processes_in_memory array where the process will be stored.
 */
void createProcess(int process_id, int memory_required, int index);

/**
 * The function `printAllProcesses` prints information about each process in memory, including process
 * ID, and memory requirements.
 * @param process_arr The array of processes in memory.
 * @param numProcesses The number of processes in memory.
 */
void printAllProcesses(Process process_arr [], int numProcesses);

/**
 * The function `shiftProcessesInMemory` shifts all processes in memory to the left by one index.
 * @param process_arr The array of processes in memory.
 * @param numProcesses The number of processes in memory.
 */
void shiftProcessesInMemory(Process process_arr [], int* numProcesses);

#endif