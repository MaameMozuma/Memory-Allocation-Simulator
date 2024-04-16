# Memory-Allocation-Simulator

### What the Project is About
This project attempts to simulate how memory allocation and deallocation work using 4 algorithms -
best fit, worst fit, first fit and next fit using forked processes.
* First-Fit:
    The program places the process at the **first** block of free space that meets the memory request
* Best-Fit:
    The program places the process at the **smallest** block of free space that meets the memory request
* Worst-Fit: 
    Memory strategy is opposite to Best-Fit. The program places the process at the **largest** block of free space that meets the memory request
* Next-Fit:
    Memory strategy works similar to First-fit but starts the search after the last allocated block.

1. Simulation begins by creating the physical memory, an array of free and used spaces
- Physical memory of 500 due to hardware limitations. Alternatively this can be switched to 1024 given the specifications of the machine running the simulation

2. Number of processes simulated is taken as a user input
- The maxium number of processes that can be simulated is 100 but is subject to change.
- Memory requests for each process is generated randomly

3. Program runs without the use of segmentation and uses random allocation of half the number of processes to simulate the creation of blocks in the memory

4. Process Address table keeps tracks of processes and their location in the physical memory through the process IDS. The physical memory contains 0s and 1s indicating free and used blocks.

5. A free list is used to keep track of free blocks in the memory

6. The other half of the processes are then used to simulate the 4 algorithms mentioned earlier.

7. Program employs the use of compaction and deallocation to simulate the algorithms effectively 

8. Statistics such as fragmentation, memory utilisation and deallocations are calculated and outputted for each algorithms to end the program.

![Alt text](https://github.com/MaameMozuma/Memory-Allocation-Simulator/assets/60921784/07fbe9ca-87dc-4275-9934-b7f2b4e74ddf "figure An image showing a high level overview of how the various data structures in this program look")


### Running the Simulation

1. First, cd into the Memory-Allocation_Simulator directory on the machine

2. Grant permissions to the simulation file running the following command
```bash
chmod +x simulate.sh
```

3. Run the simulation file with the following command
```bash
./simulate.sh
```
- Select the option you want to run using the menu numbers

#### Alternative option
5. First, cd into the Memory-Allocation_Simulator directory on the machine

6. Compile the necessary commands using the command `make` 

7. Run the program using the following command
```bash
./memorySimulation num_of_processes isCompact
```
- Replace the input with the number of processes you wish to simulate with
- Replace isCompact with 0 to simulate without compaction and 1 to simulate with
_Note:_ Run similar command as 2 if permission is denied 

8. To clean the directory out, you can run `make clean`


_Note:_ This program was tested on both Mac OS and Ubuntu 22.04. The project also does not require any sort of hosting and is simply run locally

### Overview of project
To see a brief overview of the project, access the link [here](https://youtu.be/K-UOyZRXeHI)

