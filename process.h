#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PROCESSES 50 //assume that the total memory size is 1024 MB and all processes that come are 1 MB each

typedef struct {
    pid_t pid;                // Process ID
    int memory_required;  // Memory required by the process
} Process;

Process processes_in_memory[MAX_PROCESSES]; //stores the processes that have made a request for memory to run
extern int num_of_processes; //stores the number of process currently asking/have received memory

Process *getProcessID();
void createProcess(int process_id, int memory_required, int index);
void printAllProcesses();
void shiftProcessesInMemory();