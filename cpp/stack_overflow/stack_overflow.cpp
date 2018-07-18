/*************************************************************************
	> File Name: stack_overflow.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 12 Jul 2018 04:56:07 PM CST
 ************************************************************************/

#include <stdio.h>
int main (  )
{
    char name[8];
    printf("Please type your name: ");
    gets(name);
    printf("Hello, %s!\n", name);
    return 0;

} 
