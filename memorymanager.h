#include<stdio.h>
#include<stdlib.h>
//#include"kernel.h"

int launcher(FILE* p);
int countTotalPages(FILE* p);
int countTotalInstructions(FILE* p);
void loadPage(int pageNumber, FILE* f, int frameNumber);
int findFrame();
int findVictim();
int updatePageTable();

