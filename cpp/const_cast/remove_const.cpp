/*************************************************************************
	> File Name: remove_const.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年05月24日 星期四 11时41分57秒
 ************************************************************************/

#include<iostream>
using namespace std;
    class A    
    {    
    public:    
         A()    
         {    
          m_iNum = 0;    
         }    
            
    public:    
         int m_iNum;    
    };    
            
    void foo()    
    {    
        //1. 指针指向类    
        const A *pca1 = new A;    
        A *pa2 = const_cast<A*>(pca1);  //常量对象转换为非常量对象    
        pa2->m_iNum = 200;    //fine    

        cout << pca1 << " " << pa2 << endl; //0xc73c20 0xc73c20
        //转换后指针指向原来的对象    
        cout<< pca1->m_iNum << " " << pa2->m_iNum << endl; //200 200    
           
        //-------------------------------------------------------------
        
        //2. 指针指向基本类型, const int定义已经定义了ica变量为const变量，是不可修改的变量。即时通过const_cast修改非const指针，通过也不能通过指针修改原对象，虽然非const的指针指向的对象似乎是被修改了，但并非是原对象被修改了   
        const int ica = 100;    
        const int* pica = &ica;
        //*pica = 150; //failed, can not assign a readonly data
        
        //int * ia = const_cast<int *>(&ica);    
        int * ia = const_cast<int *>(pica);    
        *ia = 200;
        cout << ia << " " << pica << " " << &ica << endl; //0x7ffcee39eb40 0x7ffcee39eb40 0x7ffcee39eb40
        cout<< *ia << " " << *pica << " " << ica << endl;   //200 200 100    

        //3. ----------------------------------------------------------
        const int* pa = new int(100);
        int* pb = const_cast<int *>(pa);
        *pb = 200;
        cout << *pa << " " << *pb << endl; //200 200

        //4. 第一种，第三种，第四种都是类似情况。即一个非const变量，由一个const指针来指向。通过const_cast修改const指针为非const指针，然后通过非const指针来修改原对象
        int ic = 100;
        const int* pic = &ic;
        int* modifier = const_cast<int *>(pic);
        *modifier = 200;
        cout << ic << endl; //200
    } 

    int main(int argc, char** argv)
    {
        foo();
    }
