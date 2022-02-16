
int exist(char* var);//Check if the variable exists in the shell memory

void addVar(char** args);//Add one var into the shell memory
void reset(char** args,int index);//Reset the shell var at the index

char* getValue(int index);//Get the value of variable at the index