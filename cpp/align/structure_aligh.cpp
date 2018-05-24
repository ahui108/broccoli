/*************************************************************************
	> File Name: structure_aligh.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年03月30日 星期五 14时55分48秒
 ************************************************************************/

#include<iostream>

struct student {
    char sex;
    int hight;
    char name[10];
};

int main(int argc, char** argv) {
    std::cout << "sizeof(student)=" << sizeof(student) << std::endl;
    //output:
    //sizeof(student)=20
    return 0;
}
