/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年06月05日 星期二 10时08分04秒
    > Desc: C++ code to call C functions, need to set the rule of compiling and calling conversion
 ************************************************************************/

extern "C"{
    #include "cHeader.h"
}

//extern "C" void print(int);

int main(int argc,char** argv)
{
    print(3);
    return 0;
}
