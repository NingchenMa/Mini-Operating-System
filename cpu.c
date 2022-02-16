#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ram.h"
#include"interpreter.h"
#include"shell.h"

#define true 1;
#define false 0;

int available = true;//By default

typedef struct CPU{
    int IP;
    int offset;//Address = IP + offset
    char IR[1000];//Store the currently executing instructions
    int quanta;//initial qunata == 2
}CPU;

CPU myCPU;//The only one CPU!

//Execute quanta number of instructions from IP
int run(int quanta){

    available = false;
    
    int count = quanta;

    for(;count > 0; count --){

        //Store the currently execution instruction into IR
        strcpy(myCPU.IR,getInstruction(myCPU.IP));//IR <-- curInstructionexec

        int error = interpreter(myCPU.IR);
        
        if(error == 1){
            available = true;
            return 1;
        }
        myCPU.IP++;
    }

    available = true;
    return 0;
}

int isAvailable(){
    return available;
}

//IP points to the ram[pc]
void setIP(int pc){
    myCPU.IP = pc;
}


