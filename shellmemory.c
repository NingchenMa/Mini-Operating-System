#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define private static
#define public

//Memory structure
typedef struct MEM{
    char var[100];//Assume each var or value can have at most 100 chars
    char value[100];
}MEM;

//Create the shell memeory structure
MEM mems[50];//Assume that there will be at most 50 variables in the shell memory at once
int count = 0;//Keep track of number of vars in the shell memory

//Check if the variable exists in the shell memory
//It exists -> return the index number
//Else -> return -1
int exist(char* var){

    char temp[100];
    strcpy(temp,var);

    for(int i =0; i<50; i++){
        //printf("%s %s\n",temp,mems[i].var);
        if(strcmp(temp,mems[i].var) == 0){
            return i;
        }
    }
    return -1;
}

//Add one variable into the shell memory
void addVar(char** args){

    char* var = args[1];
    char* value = args[2];

    strcpy(mems[count].var,var);

    strcpy(mems[count].value,value);

    count = count + 1;//Increment memory by one
}

void reset(char** args,int index){
    char* value = args[2];
    strcpy(mems[index].value,value);
}

char* getValue(int index){
    return mems[index].value;
}
