/*************************************************************************
	> File Name: sizeof.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月05日 星期三 10时30分52秒
 ************************************************************************/
#include <stdio.h>
#include<iostream>
using namespace std;

int main(int argc, char ** argv){
    printf("size of char:%d\n", sizeof(char));
    printf("size of short:%d\n", sizeof(short));
    printf("size of int:%d\n", sizeof (int));
    printf("size of long:%d\n", sizeof (long));
    printf("size of long long:%d\n", sizeof (long long));

    printf("size of float:%d\n", sizeof(float));
    printf("size of double:%d\n", sizeof(double));

    int i = 10;
    printf("int:%lld", i);
}
