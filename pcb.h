
typedef struct PCB{

    // PC = PC_page*4 + PC_offset
    int PC;//Program counter
           //PC -> the pointer to the current position of the process

    int pid;

    int numOfInstructions;
    
    //The index of array is the page number
    int pageTable[10];//Assume that the program will have at most 40 lines of code

    int PC_page;//Which page the program is currently at
    int PC_offset;//Which offset the progrsm is currently at
    int pages_max;//Total number of pages in the program

}PCB;

PCB* makePCB();
