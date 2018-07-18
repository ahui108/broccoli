/*************************************************************************
	> File Name: linux-char.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年06月06日 星期三 18时27分04秒
 ************************************************************************/

#include <iostream>
#include <cwchar>
#include <bitset>
#include <cstdio>
#include <uchar.h>
#include <clocale>

using namespace std;  
int main()  
{  
    printf("%s\n",setlocale(LC_ALL,NULL));  
    setlocale(LC_ALL,"");  
    printf("%s\n",setlocale(LC_ALL,NULL));  

    char c[]="中国h";  
    char16_t c16=u'中';  
    char32_t c32=U'中';  
    wchar_t wc[]=L"中国h";  
    cout << "char  len: " << sizeof(c) << endl;  
    cout << "char16 len: " << sizeof(c16) << endl;  
    cout << "wchar len: " << sizeof(wc) << endl;  

    bitset<8> b0(c[0]);  
    bitset<8> b1(c[1]);  
    bitset<8> b2(c[2]);  
    cout << "char 中: " << b0<<' '<< b1<<' '<< b2<< endl;  

    bitset<16> b16(c16);  
    cout << "char16中:" << b16 << endl;  

    bitset<32> b32(c32);  
    cout << "char32中:" << b32 << endl;  

    bitset<32> b3(wc[0]);  
    cout << "wchar中 :" << b3 << endl;  

    bitset<8> b4(c[6]);  
    cout << "char  h:" << b4 << endl;  

    bitset<32> b5(wc[2]);  
    cout << "wchar h:" << b5 << endl;

    wprintf(L"%ls \n", L"wpr Wide char:");  
    wcout << L"wcout Wide char" << endl;

    locale loc("zh_CN.UTF-8");  
    wcout.imbue(loc);  
    wcout << wc[0] << endl;// 想输出中，失败
    return 0;  
} 
