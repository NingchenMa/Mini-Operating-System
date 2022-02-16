#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"ram.h"

//This allows 1000 lines of String, with no lenght ristriction
//char* ram[1000];//1000 cells of memory locations

char* ram[40];//10 pages of ram

int nextAvailable = 0;

//Initiate the RAM (ram[40])-> indicates no pages of code in ram
void initRam(){
    
    for(int i = 0; i< 40; i++){
        ram[i] = NULL;
    }
    nextAvailable = 0;
}
void printRAM(){

    int index = 0;
    for(;index < 40;index++){
        printf("%s",ram[index]);
    }

    printf("\n");

}

//Set the ram[cell] = content
void setRam(int cell, char* content){

    if(cell >=40){
        printf("Not enough space in RAM\n");
        return;
    }
    ram[cell] = strdup(content);
    //printRAM();
}

//Find the index of the first encountered NULL cell in ram[40]
int findNULL(){

     for(int i = 0; i <10; i++){
        if(ram[i*4] == NULL){
            return i;
        }
    }
    return -1;
}



int addToRAM(FILE* p,int* start, int* end){


    char buffer[100];
    
    *start = nextAvailable;

    int index = *start;

    fgets(buffer,99,p);

    while(!feof(p)){

        if(index >= 1000){//Ensure that no exceeding loading will occur
            printf("Error: Not enough space in RAM to load your program(s).\n");
            initRam();
            *start = 0;
            *end = 0;
            return 7;
        }

        ram[index] = strdup(buffer);//Memory dynamically allocated by the OS
        index++;

        fgets(buffer,99,p);
    }

    nextAvailable = index;
    *end = (index-1);

    //printf("start: %d end: %d\n",*start,*end);

    return 0;
}

//Return one instruction from ram[index]
char* getInstruction(int index){
    return ram[index];
}

void clearRAM(int start,int end){
    int index = start;
    for(;index<=end;index++){
        ram[index] = NULL;
    }
}

int RamNext(){
    return nextAvailable;
}
