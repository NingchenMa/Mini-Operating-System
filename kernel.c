#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"kernel.h"
#include"ram.h"
#include"cpu.h"
#include"shell.h"
#include"memorymanager.h"

ReadyQueueNode* head = NULL;
ReadyQueueNode* tail = NULL;
int numOfPrograms = 0;

#define QUANTA 2;
//int moveHeadNext();
//int done(ReadyQueueNode* node);

//This is the main() from assignment 2
int kernel(){

    shellUI();
    return 0;
}

//Return the pid of the next process
int pid(){

    return (numOfPrograms+1);
}

//The one time call, at the start of the execution
//Start and acquire the resources it needs to run.
void boot(){
    //1. Initiate evey cell of the array to NULL
    initRam();//Make all 40 cells to be NULL
    //2. Prepares the Backing Store -> clears the Backing Store
    system("if [ -d \"BackingStore\" ]\nthen rm -r BackingStore \nfi");//Delete the old one
    system("mkdir BackingStore");//Making the new one
    
}

//Update the page-table of the one owns the victim page
void updateVictim(int frameNumber){

    ReadyQueueNode* tempNode = head;
    PCB* tempPCB;

    //Iterate through all of the processes in the ready list
    for( int i = 0; i <  numOfPrograms; i++){

        tempPCB = tempNode->pcb;

        //Iterate through the entire page table of the process
        for( int j = 0 ; j < 10; j++){
            if(tempPCB->pageTable[j] == frameNumber){
                tempPCB->pageTable[j] = -1;//Make the victim frame to be invalid
                //Here it doesn't matter if the table owns the victim frame is terminated or not
            }
        }

        tempNode = tempNode -> next;
    }
}

int myinit(char *fileName){

    /*

    FILE* p = fopen(fileName,"rt");
    
    //Laod the program into the RAM
    int error = addToRAM(p,&start,&end);//ram.c


    if (error == 7){//[error code 7] means not enought space in RAM
        head = NULL;
        tail = NULL;   //The execution will terminate
        return 7;
    }

    

    PCB* newPCB = makePCB();//pcb.c
    //Add the PCB into the ready queue
    addToReady(newPCB);//kernal.c

    */
    return 0;
    
    
}

//Add the PCB to the tail of the Ready Queue
void addToReady(PCB* pcb){

    ReadyQueueNode* newNode = (ReadyQueueNode*) malloc(sizeof(ReadyQueueNode));
    newNode->pcb = pcb;

    if(head == NULL){//If no prog in the ready queue
        head = newNode;//head --> newNode
        newNode->next = newNode;
        tail = newNode;//Circular linked list
        numOfPrograms++;
    }else{
        //Add the node to the end of the reay Queue
        newNode->next = head;
        tail->next = newNode;
        tail = newNode;
        tail->next = head;
        numOfPrograms++;
    }

}

//Return: 0 -> No process on the ready list, terminate
//Return: 1 -> Continues execute other processes
//This function: 
// 1. Remove the current head-node from the ready list
// 2. Delete its corresponding file stored in the BackingStore
int removeHeadNode(){

    //Delete its file in the Backing store:
    char temp[100];
    sprintf(temp,"rm BackingStore/%d.txt",head->pcb->pid);
    system(temp);

    //Remove the PCB from the ready list
    ReadyQueueNode* tempNode = head;
    
    if(head == tail){//Only one left on the list
        free(tempNode);//Free this PCB
        head = NULL;
        tail = NULL;

        numOfPrograms = 0;
    
        return 0 ;//No process on the ready list
                        
    }else{//More than one program is on the ready list

        head = head->next;
        tail->next = head;
        free(tempNode);//Free the PCB

        numOfPrograms--;
        return 1;//Continue execute other processes
    }
}

//Main simulation execution loop
int scheduler(){

    while(1){
    
        int quanta = QUANTA;//quanta = 2
    
        //a. If the CPU is available
        int available = isAvailable();
    
        if(available == 0){//The CPU is not available
        
            printf("The CPU is not vavailable currently, please try again later.\n");
            return 1;//Not available
                     //Probably won't happen at all!
                    
        }else{//CPU is available
        
            if(head == NULL){//a. No one loaded successfully b. All processe are finished
                
                printf("No program is in the ready list.\n");
                numOfPrograms = 0;
                return 2;//Possible error
    
            }else{//If head != null, the ready queue is not empty

                //Exceed max-pages
                //Here we have to add "=" option, because there is an "off by one" problem
                if(head->pcb->PC_page >= head->pcb->pages_max){//Terminate the process

                    //Remove the PCB from the ready list
                    int code = removeHeadNode();

                    if(code== 0){
                        return 0;
                    }else{
                        continue;//Give turn to the next process
                    }
                }

                int curFrame = head->pcb->pageTable[head->pcb->PC_page];

                //Here, the page is not loaded into the ram, pageTsble[PC_table] is invalid
                if(curFrame == -1){//If the page is NOT loaded into the ram -> [page fault] occure

                    ///////////////////////////////////////////////////////////////////////////
                    // Load the page into ram!!!
                    int freeFrame = findFrame();

                    char temp[200]; 
                    sprintf(temp,"BackingStore/%d.txt",head->pcb->pid);
                    //printf("find file: %s in the backing store\n",temp);

                    //Open the file on the backing store
                    FILE* p = fopen(temp,"rt");//The file on the "disk"

                    if(p==NULL){
                        printf("BackingStore file cannot found.\n");
                    }

                    if(freeFrame == -1){//No empty frame
                        int victimFram = findVictim();//Find a victim fram
                        loadPage(head->pcb->PC_page,p,victimFram);//Load the page to ram
                        updatePageTable(head->pcb,head->pcb->PC_page, victimFram, 1);//Update the page table for both the "me" and the "victim"

                    }else{//Found a free frame in the ram
                        loadPage(head->pcb->PC_page,p,freeFrame);
                        updatePageTable(head->pcb,head->pcb->PC_page, freeFrame, 0);//Update only one pagetable
                    }
                    /////////////////////////////////////////////////////////////

                    head->pcb->PC_offset = 0;

                    //Since the PCB is interrupted, it has lost quanta,
                    //Place the PCB at the back of the ready queue
                    head = head->next;
                    tail = tail->next;

                    continue;
                }

                //Until now, no page fault will ocuur for the current process
                curFrame = head->pcb->pageTable[head->pcb->PC_page];
                int curRamAddress = curFrame*4+head->pcb->PC_offset;

                setIP(curRamAddress);

                int page = head->pcb->PC_page;
                int offset = head->pcb->PC_offset;
                int maxLines = head->pcb->numOfInstructions;
                int curLines = page*4+offset;

                //printf("curLine: %d maxLine: %d\n",curLines,maxLines);

                //If all instructions are finnished
                if(curLines >= maxLines){//Terminate the process

                    //Remove the PCB from the ready list
                    int code = removeHeadNode();

                    if(code== 0){
                        return 0;
                    }else{
                        continue;//Give turn to the next process
                    }

                }else if(maxLines - curLines == 1){//Execute only one line

                    quanta = 1;

                    setIP(curRamAddress);//IP <-- curPC

                    int error = run(quanta);

                    if(error == 1){//Quit
                        head->pcb->PC_page = head->pcb->pages_max+1;
                    }
                    //Update the PC after quanta is finished
                    head->pcb->PC_page = head->pcb->pages_max+1;//Will terminate this process in the next turn
                  
                    tail = tail->next;
                    head = head->next;
                    
                }else{//Execute 2 lines

                    setIP(curRamAddress);

                    int error = run(quanta);

                    if(error == 1){//Quit
                        head->pcb->PC_page = head->pcb->pages_max+1;
                    }else{

                        //Update the PC after quanta is finished
                        head->pcb->PC_offset = (head->pcb->PC_offset + quanta) % 4;

                        if(head->pcb->PC_offset == 0){//A new page
                            head->pcb->PC_page++;
                        }

                        tail = tail->next;
                        head = head->next;
                    }
                }
                
            }//Note: The RAM will be cleared entirely in the interpreter() function!

            //return 0;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////

/*
int main(){ 

    int error = 0;
    boot();
    error = kernel();
    return error;
}*/



