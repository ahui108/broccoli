#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <wchar.h>
#include <bitset>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <ctype.h>
#else
#include <iconv.h>
#endif

int MBS2Wstring(const char* in, const int len, std::wstring& wstr)
{
	int rc = -1;
#if defined(_WIN32) || defined(WIN32) // ACP to UTF-16
    int wlen = MultiByteToWideChar(CP_ACP, 0, in, len, NULL, 0);  
    wchar_t* pwcs = new wchar_t[wlen + 1];  
	memset(pwcs, 0, (wlen + 1) * sizeof(wchar_t));
    //printf("wcs len:%d\n", len);
    MultiByteToWideChar(CP_ACP, 0, in, len, pwcs, len);  
	wstr = std::wstring(pwcs);
    delete []pwcs;  
	rc = wlen;
#else // locale to UTF-32
    //setlocale(LC_ALL, "zh_CN.GB2312"); 
    setlocale(LC_ALL, ""); 
    //printf("str=%s\n", str.c_str());
    size_t wlen = mbstowcs(NULL, in, 0); 
    if (wlen == (size_t)-1)
    {
		printf("invalid conversion to wstring!\n");
		rc = -1;
    }
	else
	{
		//printf("wcs len:%zd\n", size);
		wchar_t* pwcs = new wchar_t[wlen + 1]; 
		memset(pwcs, 0, (wlen + 1) * sizeof(wchar_t));
		rc = mbstowcs(pwcs, in, wlen); 
		wstr = std::wstring(pwcs);
		delete []pwcs;
	}
#endif
	return rc;
}

int Wstring2MBS(const std::wstring& wstr, char* out, const int len)
{
	int rc = -1;
#if defined(_WIN32) || defined(WIN32) //UTF-16 to ACP
    int mlen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL); 
    //printf("mbs len:%d\n", len);
    char* pmbs = new char[mlen + 1]; 
	memset(pmbs, 0, mlen + 1);
    WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), wstr.length(), pmbs, mlen, NULL, 0);
	rc = len > mlen ? mlen : len;
	memcpy(out, pmbs, rc);
    delete []pmbs;
#else //UTF-32 to locale
    //setlocale(LC_ALL, "zh_CN.UTF8"); 
    setlocale(LC_ALL, ""); 
    size_t mlen = wcstombs(NULL, wstr.c_str(), 0); 
    if (mlen == (size_t)-1)
    {
        printf("invalid conversion to string!\n");
        rc = -1;
    }
    else
	{
		//printf("mbs len:%zd\n", size);
		char* pmbs = new char[mlen + 1]; 
		memset(pmbs, 0, mlen + 1);
		wcstombs(pmbs, wstr.c_str(), mlen); 
		rc = len > mlen ? mlen : len;
		memcpy(out, pmbs, rc);
		delete []pmbs;
	}
#endif
	return rc;
}

std::wstring to_wstring(const std::string &str)
{
    std::wstring wstr;  
#if defined(_WIN32) || defined(WIN32)//ACP to UTF-16
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);  
    wchar_t* pwchar = new wchar_t[len + 1];  
    //printf("wcs len:%d\n", len);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwchar, len);  
    pwchar[len] = L'\0';  
    wstr = std::wstring(pwchar);  
    delete []pwchar;  
#else //locale to UTF-32
    //setlocale(LC_ALL, "zh_CN.GB2312"); 
    setlocale(LC_ALL, ""); 
    //printf("str=%s\n", str.c_str());
    size_t size = mbstowcs(NULL, str.c_str(), 0); 
    if (size == (size_t)-1)
    {
        printf("invalid conversion to wstring!\n");
        return L"";
    }

    //printf("wcs len:%zd\n", size);
    wchar_t* pwcs = new wchar_t[size + 1]; 
    size = mbstowcs(pwcs, str.c_str(), size + 1); 
    pwcs[size] = L'\0'; 
    wstr = std::wstring(pwcs);
    delete []pwcs;
#endif
    return wstr;  
}

std::string to_string(const std::wstring &wstr)
{
    std::string str;
#if defined(_WIN32) || defined(WIN32) //UTF-16 to ACP
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL); 
    //printf("mbs len:%d\n", len);
    char* pstr = new char[len + 1]; 
    WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), wstr.length(), pstr, len, NULL, 0);
    pstr[len] = '\0';
    str = std::string(pstr);
    delete []pstr;
#else //UTF-32 to locale
    //setlocale(LC_ALL, "zh_CN.UTF8"); 
    setlocale(LC_ALL, ""); 
    size_t size = wcstombs(NULL, wstr.c_str(), 0); 
    if (size == (size_t)-1)
    {
        printf("invalid conversion to string!\n");
        return "";
    }
    //printf("mbs len:%zd\n", size);
    char* pstr = new char[size + 1]; 
    wcstombs(pstr, wstr.c_str(), size + 1); 
    str[size] = '\0'; 
    str = std::string(pstr);
    delete []pstr;
#endif
    return str;
}


void print_wchar(const wchar_t* wstr, int len)
{
#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
    std::bitset<16> bx;
#else
    std::bitset<32> bx;
#endif
    //int len = wcslen(wch);
    //int len = wstr.length();
    printf("wcs len=%d\n", len);
    for(int i = 0; i < len; i++)
    {
        bx = wstr[i];
        std::cout << bx << " ";
    }
    printf("\n");
}

void print_char(const char* str, int len)
{
    std::bitset<6> bx;
    //int len = str.length();
    printf("mbs len=%d\n", len);
    for(int i = 0; i < len; i++)
    {
        bx = str[i];
        std::cout << bx << " ";
    }
    printf("\n");
    printf("string=%s\n", str);
}

void print_UTF16(const char* str, int len)
{
	typedef char UCHAR[2];
	UCHAR* uchar = (UCHAR *)str;
    std::bitset<16> bx;
    //int len = wcslen(wch);
    //int len = wstr.length();
    //printf("wstring len=%d\n", len);
    for(int i = 0; i < len/2; i++)
    {
        memcpy(&bx, uchar + i, 2);
        std::cout << bx << " ";
    }
    printf("\n");
}

int WCS2UTF16(const wchar_t* inbuf, int inlen, char* outbuf, int outlen)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)   
    memcpy_s((wchar_t*)outbuf, outlen, inbuf, inlen);  
	printf("inlen:%d\n", inlen);
    return inlen ;  
#else
    size_t rc;  
    iconv_t env;  
    env = iconv_open("UCS-2", "UTF-32");  
    //env = iconv_open("UTF-16", "WCHAR_T");  
    //env = iconv_open("UTF-16", "UTF-32");  
    if (env == (iconv_t)-1)  
    {  
    	printf("iconv_open WCHAR_T->UTF16 error, %s %d\n", strerror(errno),errno);  
        rc = -1;
    }  
    else
    {
        printf("inlen:%d, outlen:%d\n", inlen, outlen);
        size_t inL = inlen;
        size_t outL = outlen;
        rc = iconv(env, (char**)(&inbuf), &inL, (char**)(&outbuf), (size_t*)(&outL));  
        if (rc ==(size_t)-1)  
        {  
            printf("iconv WCHAR_T->UTF16 error, %s %d\n", strerror(errno), errno);  
        } 

        iconv_close(env);  
		printf("rc:%zd, inlen:%d, inL:%zd, outlen:%d, outL:%zd\n", rc, inlen, inL, outlen, outL);	
    }

    return (int)rc;  
#endif
}

int main(int argc, char** argv)
{
    printf("please input characters to convert...\n");
    char ch[128] = "\0";
    scanf("%s", ch);

    printf("\n**convert mbs(Linux/UTF-8, Win/ACP) to wstring...\n");
    //std::wstring wstr = to_wstring(std::string(ch));
	std::wstring wstr;
	MBS2Wstring(ch, 128, wstr);
    print_wchar(wstr.c_str(), wstr.length());

    /*
    int mode = fwide(stdout, 1); //set wide char oriented
    //printf("fwide:%d\n", mode);
    freopen(NULL, "w", stdout);
    wprintf(L"wstring=%ls\n", wstr.c_str());

    mode = fwide(stdout, -1); //set multi bytes oriented
    freopen(NULL, "w", stdout);
    //printf("fwide:%d\n", mode);
    */

	printf("\n**convert wstring to UTF-16...\n");
	//wchar_t inbuf[] = L"中文编码";
	wchar_t* inbuf = const_cast<wchar_t *>(wstr.c_str());
	print_wchar(inbuf, wcslen(inbuf));
    char outbuf[1024] = "\0";
	int outlen = sizeof(outbuf);
	int inlen = wcslen(inbuf) * sizeof(wchar_t);
    int len = WCS2UTF16(inbuf, inlen, outbuf, outlen);
	if (len != -1)
	{
		int len = inlen;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#else
		len = inlen/2;
#endif
		print_UTF16(outbuf, len);	
	}

    printf("\n**convert wstring to mbs(Linux/UTF-8, Win/ACP)...\n");
    //std::string str = to_string(wstr);
    char str[128] = "\0";
    len = Wstring2MBS(wstr, str, 128);
    //print_char(str.c_str(), str.length());
    print_char(str, len);
}

