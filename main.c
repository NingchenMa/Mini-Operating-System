#include"kernel.h"
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main(){ 

    int error = 0;
    boot();             //First: actions performed by boot
    error = kernel();   //Second: actions performed by kernel
    return error;
}