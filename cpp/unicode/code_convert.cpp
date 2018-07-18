#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#include <ctype.h>
#else
#include <iconv.h>
#include <locale.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
int convert_charset(unsigned int from_charset, unsigned int to_charset, char *inbuf, int inlen, char *outbuf, int outlen)
{
	int rc = 0;
    int wcslen = MultiByteToWideChar(from_charset, 0, inbuf, inlen, NULL, 0);
	wchar_t* pwchar = new wchar_t[wcslen + 1];
	MultiByteToWideChar(from_charset, 0, inbuf, inlen, pwchar, wcslen);
	pwchar[wcslen] = L'\0';
	int mbslen = WideCharToMultiByte(to_charset, 0, pwchar, wcslen, NULL, 0, NULL, NULL);
	if (outlen < mbslen) 
		rc = -1;
	else
		WideCharToMultiByte(to_charset, 0, pwchar, wcslen, outbuf, outlen, NULL, 0);   
	delete []pwchar; 
	return rc;
}
#else
int convert_charset(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen)
{
	int rc = 0;
    char *pbin = inbuf;
    char *pbout = outbuf;
	char **pin = &inbuf;
	char **pout = &outbuf;

	iconv_t cd;
	cd = iconv_open(to_charset,from_charset);
	if (cd == 0)
    { 
        printf("iconv_open failed, %s %d\n", strerror(errno), errno);
        rc = 0;
    }
    else
    {
        size_t inL = inlen;
        size_t outL = outlen;
        printf("in: %s, inlen:%d, out:%s, outlen:%d\n", inbuf, inlen, outbuf, outlen);
        rc = iconv(cd, pin, &inL, pout, &outL);
        if (rc == -1)
        {
            printf("iconv failed, %s %d, inleft:%zd, outleft:%zd\n", strerror(errno), errno, inL, outL);
        }

        iconv_close(cd);
        printf("pbin:%s, in:%s, inlen:%d, inL:%zd, pbout:%s, out:%s, outlen:%d, outL:%zd\n", pbin, inbuf, inlen, inL, pbout, outbuf, outlen, outL);
    }

	return rc;
}
#endif    

int main(int argc, char** argv)
{
	int rc = 0;
    //setlocale(LC_ALL, "");
	wchar_t in[256] = L"中文编码";
    //char szIn[512] = "abc";
	char out[256] = "\0";
	int inlen = wcslen(in);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	int from_cp = 1200;//utf-16
	int to_cp = 1200; //utf-16
	rc = convert_charset(from_cp, to_cp, (char *)in, inlen, out, 256);
#else
	rc = convert_charset((char *)"UTF-32", (char *)"UCS-2", (char *)(in), inlen, out, 256);
#endif

    if (rc == -1)
        printf("u2g failed!\n");
	printf("unicode-->gb2312 out=%s\n",out);
}
