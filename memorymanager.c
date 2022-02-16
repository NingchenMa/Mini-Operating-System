#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include"ram.h"
#include"kernel.h"

//Helper functions//

int ceilling(float f){

    int try = 0;

    while(try<f){
        try++;
    }
    return try;
}

//Return the seek distance (counted in number of chars) to a certain line
int seekNum(FILE* p,int line){

    fseek(p,0,SEEK_SET);

    int countChar = 0;

    for(int i = 0; i< line;i++){

        char temp;
        temp = fgetc(p);

        while(temp != '\n'){
            countChar++;
            temp = fgetc(p);
        }

    }

    return countChar+line;
}

//Returns the total number of pages needed by a program.
int countTotalPages(FILE* p){

    char temp[1000];
    int countLines = 0;
    
    fgets(temp,999,p);

    while(!feof(p)){
        countLines++;
        fgets(temp,999,p);
    }

    int countPages = (int)ceilling(countLines/4.0);//Needed number of pages

    fseek(p,0,SEEK_SET);

    return countPages;
}

//Return the number of lines/instructions in the file
int countTotalInstructions(FILE* p){

    char temp[1000];
    int countLines = 0;
    
    fgets(temp,999,p);

    while(!feof(p)){
        countLines++;
        fgets(temp,999,p);
    }
    fseek(p,0,SEEK_SET);
    return countLines;
}
//FILE* f -> points to the buginning of the file in the backing store.
//pageNumber -> the desired page from the backing store
//The function loads the [4 lines] of code from the page into the frame in ram[]
void loadPage(int pageNumber, FILE* f, int frameNumber){

    //printf("load page %d into frame %d\n",pageNumber,frameNumber);

    int fileLine = pageNumber*4;//Copy the file from which line

    int charNum = seekNum(f,fileLine);

    int ramLine = frameNumber*4;
    
    fseek(f,charNum,SEEK_SET);//Set the file pointer to the desired line position

    //Load 4 lines of code from the file into the given ram frame
    char temp[1000];
    fgets(temp,999,f);

    //No need to worry about the [incomplete-page] error here, will handle at the execution stage
    for(int i  = 0; i < 4; i++){
        
        setRam(ramLine,temp);//ram[ramLine] = temp
        ramLine++;
        fgets(temp,999,f);
    }

}

//Use the FIFO technique to search ram[] for a free frame (eqaul to NULL).
//If one exists -> return index
//Else -> return -1
int findFrame(){
    return findNULL();
}

//This function invokes only when [findFrame == -1].
int findVictim(PCB* p){

    int randomIndex = rand() % 10;//Pick a random frame number in the range of (0-9)

    int exist;

    for(int j = 0; j <10; j ++){

        randomIndex = (randomIndex+j) % 10;//Increment one for each turn

        for(int i = 0; i < 10; i++){
            if(randomIndex == p->pageTable[i]){
                exist = 1;
            }
        }

        if(exist != 1){
            return randomIndex;
        }
    }

    return -1;//Problem occurs

}

int updatePageTable(PCB* p,int pageNumber, int frameNumber, int victimFrame){
    
    if(victimFrame == 1){
        //Update the victim pagetable, need a linear search
        updateVictim(frameNumber);//This function is inside kernel.c
        p -> pageTable[pageNumber] = frameNumber;

    }else{
        p -> pageTable[pageNumber] = frameNumber;
    }
    return 0;
}

//Copy contents from the source_file to the dest_file
void copyFiles(FILE* sf, FILE* df){

    if(sf == NULL || df == NULL){
        printf("Non-existing files.\n");
        return;
    }

    char temp[100];

    fgets(temp,99,sf);
    while(!feof(sf)){
        fprintf(df,"%s",temp);
        fgets(temp,99,sf);
    }

    fclose(sf);
    fclose(df);
}

//////////////////////////////////////////////////////////////
//Return: 1 -> successfully launching the program
//Return: 0 -> otherwise
int launcher(FILE* p){//The file in the curDirectory, not in the backing store

    //Making a PCB for the process
    

    int lineNum = countTotalInstructions(p);
    int pageNum = countTotalPages(p);

    //printf("This number of instructions: %d\n",countTotalInstructions(p));

    PCB* newPCB = makePCB(pageNum,pid(),lineNum);
    
    addToReady(newPCB);//Add the newly created PCB onto the reay queue

    //1. Copy the entire file into the backing store

    //a. Create a file in the backing store directory
    char temp[200]; 
    sprintf(temp,"cd BackingStore\ntouch %d.txt",newPCB->pid);
    system(temp);
    //b. Copy the file
    sprintf(temp,"BackingStore/%d.txt",newPCB->pid);//File name inside
    FILE* sf = p;//Read the initial file
    FILE* df = fopen(temp,"wt");//Write to the new file

    //printf("Open two files done!\n");

    if(df == NULL){
        printf("The file is not in the backing-store.\n ");
        return 1;
    }

    copyFiles(sf,df);//This function also closes two file pointers after the copy

    //2. Close the file pointer to the original file
    //Alreayd done in the previous step

    //3. Open the file in the backing store
    FILE* f = fopen(temp,"rt");//Read the file

    //4. Loads 2 pages into the ram initially (by default)

    if(newPCB->pages_max < 2){//Load only one page into the ram

        for( int i = 0; i< 1; i++){

            int emptyFrame = findFrame();//Find an empty fram in the ram
            
            if(emptyFrame == -1){//No empty(NULL) frame in the ram
                int victimFrame = findVictim(newPCB);//When at initialization, no pages is assigned to any frame, I suppose no problem wii happen here
                loadPage(i,f,victimFrame);//Laod the page [i] into the victim frame

                updatePageTable(newPCB,i, victimFrame, 1);
            }else{//emptyFrame != -1
                loadPage(i,f,emptyFrame);
           
                updatePageTable(newPCB,i,emptyFrame,0);
            }
        }

    }else{//Laod 2 pages into the ram

        for( int i = 0; i< 2; i++){

            int emptyFrame = findFrame();//Find an empty fram in the ram

            if(emptyFrame == -1){//No empty(NULL) frame in the ram
                int victimFrame = findVictim(newPCB);//When at initialization, no pages is assigned to any frame, I suppose no problem wii happen here
                loadPage(i,f,victimFrame);//Laod the page [i] into the victim frame

                updatePageTable(newPCB,i, victimFrame, 1);
            }else{//emptyFrame != -1
                
                loadPage(i,f,emptyFrame);
                updatePageTable(newPCB,i,emptyFrame,0);
            }
        }

    }

    return 0;
}

