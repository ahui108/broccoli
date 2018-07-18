#include <iostream>
#include <stdint.h>
#include <stddef.h>

class Base
{
public:
    Base(int iBase);
    ~Base();
    Base(const Base & base);
    Base & operator=(const Base & base);

    //virtual void BaseFun1();
    void BaseFun1();

private:
    char m_base;

};

class Derive: public Base
{
public:
    Derive(int iDerive);
    ~Derive();
    Derive(const Derive & derive);
    Derive & operator=(const Derive & derive);
 
    void DeriveFun1();

private:
    int m_derive;
};

/**********************************************************/
Base::Base(int iBase):m_base(iBase){
}

Base::~Base(){}

Base::Base(const Base & base){
    this->m_base = base.m_base;
}

Base & Base::operator=(const Base & base){
    this->m_base = base.m_base;
    return *this;
}

void Base::BaseFun1(){
    std::cout << "call Base::BaseFun1" << std::endl;
    std::cout << "m_base " << m_base << std::endl;
}

/***********************************************************/
Derive::Derive(int iDerive):Base(iDerive), m_derive(iDerive) {
}

Derive::~Derive(){};

Derive::Derive(const Derive & derive):Base(derive){
    this->m_derive = derive.m_derive;
}

Derive & Derive::operator=(const Derive & derive){
    this->m_derive = derive.m_derive;
}

void Derive::DeriveFun1(){
    std::cout << "Call Derive::DeriveFun1()" << std::endl;
    std::cout << "Derive::m_derive = " << m_derive << std::endl;
}

/*************************************************************/
class CNULL{
};

class A{
public:
	int a;
private:
	char b; //algin
};

class B: public A{
public:
	/*
	int d;
	char c;
	*/ //sizeof(B) = 16 = 4 + 1 + 4 + 1 ~= 16;
	char c;
	int d; //sizeof(B) = 12 = 4 + 1 + 1 + 4 ~= 12;
};

class C: virtual public A{
public: 
	int d;
	char c;
	//virtual void fun();
	//void fun();
}; 

/************************************************************/
class Test{
public: 
	char a;
	virtual void get();
}; //sizeof(Test) = 16 = 8 + 1 ~=16

class Test2: public Test{
public:
	int a;
};//sizeof(Test2) = 16 = 8 + 1 + 4 ~= 16

/************************************************************/
int main(int argc, char ** argv)
{
	char * ptrChar = NULL;
	std::cout << "sizeof(char *) = " << sizeof(char *) << std::endl;
	std::cout << "sizeof(ptrChar) = " << sizeof(ptrChar) << std::endl;
    std::cout <<"*************************************************"<<std::endl;

	char * ptrszChar[10] = {};
	std::cout << "sizeof(ptrszChar) = " << sizeof(ptrszChar) << std::endl;
    std::cout <<"*************************************************"<<std::endl;

	char (*ptrC)[10] = NULL;
	std::cout << "sizeof(ptrC) = " << sizeof(ptrC) << std::endl;
    std::cout <<"*************************************************"<<std::endl;

	char szChar[10] = {};
	std::cout << "sizeof(szChar) = " << sizeof(szChar) << std::endl;
    std::cout <<"**************************************************"<<std::endl;
	
	int szInt[10] = {};
	std::cout << "sizeof(szInt) = " << sizeof(szInt) << std::endl;
    std::cout <<"************************************************"<<std::endl;

	std::cout << "sizeof(int[10]) = " << sizeof(int[10]) << std::endl;
    std::cout <<"************************************************"<<std::endl;

	std::cout << "sizeof(char) = " << sizeof(char) << std::endl;
	std::cout << "sizeof(wchar_t) = " << sizeof(wchar_t) << std::endl;
	std::cout << "sizeof(short) = " << sizeof(short) << std::endl;
	std::cout << "sizeof(size_t) = " << sizeof(size_t) << std::endl;
	std::cout << "sizeof(int) = " << sizeof(int) << std::endl;
	std::cout << "sizeof(long int) = " << sizeof(long int) << std::endl;
	std::cout << "sizeof(long long int) = " << sizeof(long long int) << std::endl;
	std::cout << "sizeof(float) = " << sizeof(float) << std::endl;
	std::cout << "sizeof(double) = " << sizeof(double) << std::endl;
	std::cout << "sizeof(long double) = " << sizeof(long double) << std::endl;
	std::cout << "sizeof(intmax_t) = " << sizeof(intmax_t) << std::endl;
	std::cout << "sizeof(ptrdiff_t) = " << sizeof(ptrdiff_t) << std::endl;
    std::cout <<"***********************************************"<<std::endl;

	std::cout << "sizeof(Base) = " << sizeof(Base) << std::endl;
	std::cout << "sizeof(Derive) = " << sizeof(Derive) << std::endl;
    std::cout <<"***********************************************"<<std::endl;

	std::cout << "sizeof(CNULL) = " << sizeof(CNULL) << std::endl;
	std::cout << "sizeof(A) = " << sizeof(A) << std::endl;
	std::cout << "sizeof(B) = " << sizeof(B) << std::endl;
	std::cout << "sizeof(C) = " << sizeof(C) << std::endl;
    std::cout <<"***********************************************"<<std::endl;

	std::cout << "sizeof(Test) = " << sizeof(Test) << std::endl;
	std::cout << "sizeof(Test2) = " << sizeof(Test2) << std::endl;
}

