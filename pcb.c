#include<stdlib.h>
#include<stdio.h>
#include<string.h>
//#include"pcb.h"
#include"kernel.h"

//int id;

//Create a PCB instance
PCB* makePCB(int pageNum, int pid, int numOfInstructions){

    //Making a new PCB
    PCB* newPCB = (PCB*)malloc(sizeof(PCB));

    //Initiate the page table
    for(int i = 0; i < 10; i++){
        newPCB->pageTable[i] = -1;
    }

    //Set for the PID
    newPCB->pid = pid;//Useful for creating the backing-store file-name

    newPCB->PC_page = 0;//current page initially at page 0
    newPCB->PC_offset = 0;//Current offset initially with 0 

    //Set the total number of pages needed by the process
    newPCB->pages_max = pageNum;//This is a constant

    //Set num of instructions in the script
    newPCB->numOfInstructions = numOfInstructions;

    return newPCB;
}
