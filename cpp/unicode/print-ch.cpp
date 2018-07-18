/*************************************************************************
	> File Name: print-ch.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年06月06日 星期三 19时56分08秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <bitset>
#include <iostream>

int main(int argc, char** argv)
{
    char ch[128] = "\0";
    scanf("%s", ch);
    printf("%s\n", ch);
    std::bitset<8> bx;
    int len = strlen(ch);
    for(int i = 0; i < len; i++)
    {
        bx = ch[i];
        //printf("%02X ", b);
        std::cout << bx << " ";
    }
    printf("\n");

    return 0;
}

