#include "process.h"

int num_of_processes = 0; //initialising number of processes currently in the `processes_in_memory` array

Process *getProcessID(){
    Process *process = (Process *)malloc(sizeof(Process)); //creating a process struct
    process->pid = fork(); //forking a child process

    if(process->pid == -1){ //if process failed to fork
        printf("Error creating process\n");
        exit(1);
    }
    else if(process->pid == 0){ //if process was successfully forked
        process->pid = getpid();
        exit(0); //exit the child process
    }
    else{
        return process;
    }
}

void createProcess(int process_id, int memory_required, int index){
    processes_in_memory[index].pid = process_id;
    processes_in_memory[index].memory_required = memory_required;
    num_of_processes += 1;
}

void printAllProcesses(Process process_arr [], int numProcesses){
    printf("Processes in memory:\n");
    printf("+---------------+---------------+\n");
    printf("| Process ID    | Memory Request|\n");
    printf("+---------------+---------------+\n");
    for (int i = 0; i < numProcesses; i++){
        printf("| %-13d | %-13d |\n", process_arr[i].pid, process_arr[i].memory_required);
    }
    printf("+---------------+---------------+\n");
}

void shiftProcessesInMemory(Process process_arr [], int* numProcesses){
    for (int i = 0; i < *numProcesses; i++){
        process_arr[i] = process_arr[i+1]; 
    }
    *numProcesses -= 1;
}

