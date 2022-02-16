#include"pcb.h"

//Singlely linked list
typedef struct ReadyQueueNode{

    PCB* pcb;
    struct ReadyQueueNode* next;

}ReadyQueueNode;

int myinit(char *fileName);
void addToReady(PCB* pcb);
int scheduler();
int pid();
void updateVictim(int frameNumber);
int kernel();
void boot();