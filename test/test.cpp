/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年02月28日 星期二 19时54分24秒
 ************************************************************************/

#include<iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char** argv)
{
    /*
    char sz[2][5] = {"1111", "2222"};
    printf("sz[0]:%s", (char *)(sz + 0) );
    printf("sz[1]:%s", (char *)(sz + 1) );
    */

    if (0x2012D32F > 0xffffffff){
        printf(">>>>>>");
    } else {
        printf("<<<<<<");
    }

    printf("end");

}
