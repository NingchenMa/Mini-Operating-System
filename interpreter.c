#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"shellmemory.h"
#include"shell.h"
#include"kernel.h"
#include"ram.h"
#include"memorymanager.h"

#define private static


int interpreter(char* arg);

private void help(){

    char* allCmd = "\nCOMMAND\t\t\t\tDESCRIPTION\n\nhelp\t\t\t\tDisplay all the commands\n"
    "quit\t\t\t\tTerminates the shell\n"
    "set VAR STRING\t\t\tAssigns a value to shell memory\n"
    "print VAR\t\t\tDisplay the string assgned to VAR\n"
    "run SCRIPT.TXT\t\t\tExecutes the file SCRIPT.TXT\n"
    "exec p1 p2 p3\t\t\tExecutes concurrent programs\n";

    printf("%s",allCmd);
}
private int quit(){
    printf("%s\n","Bye!");
    return 1;
}
private void set(char** args){
    //If exists
    if( exist(args[1]) != -1 ){
        int index = exist(args[1]);//Get the index of the variable in the shell memory
        reset(args,index);
        //printf("Exist!\n");
        
    }else{//If existed alresdy
        //printf("Dosen't Exist!\n");
        addVar(args);
    }

}

private void print(char** args){
    char* var = args[1];
    int index = exist(var);

    if(index != -1){
        printf("%s = %s\n",var,getValue(index));
    }else{
        printf("Variable \"%s\" does not exist.\n",var);
    }
}

//Helper method to for cmd "run"
private void shell(FILE* p,char* fileName){

    char arg[1000];//Assume all input commands are less than 1000 characters
    char buffer[100];//Used to get rid of unwanted chars

    fgets(arg,999,p);

    for(;1 && !feof(p);){
      
        int code = interpreter(arg);

        if(code == 1){

            //exit(1);//Terminates the shell
            printf("(The script shell of \"%s\" is terminated.)\n",fileName);
            break;

        }

        fgets(arg,999,p);

        if(feof(p)){
            break;
        }
    }
    
}

private void run(char** args){

    char fileName[100];
    strcpy(fileName,args[1]);


    FILE* p = fopen(fileName,"rt");

    if(p == NULL){
        printf("Script <%s> not found.\n",fileName);
        return;
    }else{
        shell(p,fileName);
    }

    fclose(p);
}

//Will return error number
private int exec(char** args,int num){

    char* name1;
    char* name2;
    char* name3;
    FILE* p1;
    FILE* p2;
    FILE* p3;

    if(num == 2){//1 script
    
        name1 = args[1];
        p1 = fopen(name1,"rt");

        if(p1 == NULL){//If file not found
            printf("Script <%s> not found.\n",name1);
            return 2;//Script not found
        }else{
            //Launch the program
            launcher(p1); 

            scheduler();//Run the loaded program

            initRam();//Clear the Ram

            return 0;//Success
        }

    }else if(num ==3){//2 scripts

            p1 = fopen(args[1],"rt");
            p2 = fopen(args[2],"rt");
            name1 = args[1];
            name2 = args[2];

            if(p1==NULL){

                printf("Script <%s> not found.\n",name1);
                return 2;//Script not found error

            }else if(p2==NULL){
                printf("Script <%s> not found.\n",name2);
                return 2;//Script not found error

            }else{//Both scripts can be found

                //Launch the program
                launcher(p1); 
                launcher(p2);

                scheduler();//Run the loaded program

                initRam();//Clear the Ram

                return 0;//Success
            }

        
        
    }else{//3 scripts

        p1 = fopen(args[1],"rt");
        p2 = fopen(args[2],"rt");
        p3 = fopen(args[3],"rt");
        name1 = args[1];
        name2 = args[2];
        name3 = args[3];
        
            if(p1 == NULL){
                printf("Script <%s> not found.\n",name1);
                return 2;//Script not found error

            }else if(p2==NULL){
                printf("Script <%s> not found.\n",name2);
                return 2;//Script not found error
                
            }else if(p3==NULL){
                printf("Script <%s> not found.\n",name3);
                return 2;//Script not found error

            }else{//All scripts exists

                //Launch the program
                launcher(p1); 
                launcher(p2);
                launcher(p3);

                scheduler();//Run the loaded program

                initRam();//Clear the Ram

                return 0;//Success
            }
    }
}

private void unknown(){
    printf("Unknown command\n");
}

//@arg:commands --> an array of arguments
//@arg:num --> number of input arguments
int interpreter(char* arg){

    int num = numOfArgs(arg);
    char** commands = parse(arg,num);

    const int numOfCommands = 6;
    char* listOfCmd[numOfCommands]; // An array of command strings

    listOfCmd[0] = "help";
    listOfCmd[1] = "quit";
    listOfCmd[2] = "set";
    listOfCmd[3] = "print";
    listOfCmd[4] = "run";
    listOfCmd[5] = "exec";

    char first[100];
    strcpy(first,commands[0]);
    //help
    if( (strcmp(first,listOfCmd[0]) == 0)){

        if(num != 1){
            printf("Wrong input argument. Did you mean \"help\"?\n");
        }else{
            help();
        }
        return 0;
    //quit
    }else if((strcmp(first,listOfCmd[1] )) == 0){
        if(num != 1){
            printf("Wrong input argument. Did you mean \"quit\"?\n");// -2 -> syntax error
        }else{
            return quit();//Terminate the shell
        }
        return 0;//Special case
        
    //set
    }else if((strcmp(first,listOfCmd[2]) == 0)){

        if(num != 3){
            printf("Wrong number of arguments! 2 arguments are expected for the command \"set\".\n");
            return -2;
        }else{
            set(commands);
        }
        return 2;
    //print
    }else if((strcmp(first,listOfCmd[3]) == 0)){
        if(num != 2){
            printf("Wrong number of arguments! 1 argument is expected for the command \"print\".\n");
            return -2;
        }else{
            print(commands);
        }
        return 3;
    //run
    }else if(strcmp(first,listOfCmd[4]) == 0){
        if(num != 2){
            printf("Wrong number of arguments! 1 file name is expected for \"run\" command.\n");
            return -2;
        }else{
            run(commands);
        }
        return 4;//run == 4
    //exec
    }else if(strcmp(first,listOfCmd[5]) == 0){
        
        //Wrong number of input arguments
        if((num != 2)&&(num != 3)&&(num != 4)){
            printf("Wrong number of arguments! 1 to 3 sripts input are expected for \"exec\".\n");
            return -2;
        }else{

           exec(commands,num);
           //printf("exec finish\n");

        }
        return 5;//exec == 5


    //unknown
    }else{
        unknown();
        return -1;
    }
}



