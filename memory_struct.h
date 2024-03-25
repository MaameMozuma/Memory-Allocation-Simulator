#define MEMORY_SIZE 20 // Total size of memory in bytes
#define BLOCK_SIZE 4 //size per memory block in bytes
#define NUM_BLOCKS (MEMORY_SIZE/BLOCK_SIZE) //size per memory block in bytes

//structure for memory 
typedef struct memoryBlock{
    int blockNum;
    int startAddress;
    int size;
    int processIDs [BLOCK_SIZE]; //-1 for unallocated
    int processSizes [BLOCK_SIZE];
    int status; //0 for free, 1 for allocated
    struct memoryBlock* next;
}memoryBlock;

void initializeMemory(memoryBlock* firstNode);
void printMemory(memoryBlock* firstNode);
