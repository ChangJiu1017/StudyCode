#include "linux_math.h"
#include <stdio.h>

void hello(){
    printf("hello from shared library!\n");

}

int add(int a, int b)
{
    return a+b;
}