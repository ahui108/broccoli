/*************************************************************************
	> File Name: print-ch.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年06月06日 星期三 19时56分08秒
 ************************************************************************/

#include <stdio.h>
#include <cwchar>
#include <string.h>
#include <bitset>
#include <iostream>
//#include <locale>

int main(int argc, char** argv)
{
    //printf("%s\n",setlocale(LC_ALL,NULL));
    setlocale(LC_ALL, "");
    wchar_t wch[128] = L"\0";
    wscanf(L"%ls", wch);
    wprintf(L"%ls\n",wch);
    //printf("%s\n",setlocale(LC_ALL,NULL));
    #if 0
    printf("汉字输入\n");
    int mode = fwide(stdout, 1);
    printf("fwide:%d\n", mode);
    freopen(NULL, "w", stdout);
    wprintf(L"中文字符\n");
    wprintf(L"****wch****\n");
    #endif

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
    std::bitset<16> bx;
#else
    std::bitset<32> bx;
#endif
    int len = wcslen(wch);
    for(int i = 0; i < len; i++)
    {
        bx = wch[i];
        //printf("%02X ", b);
        std::cout << bx << " ";
    }
    printf("\n");

    return 0;
}

