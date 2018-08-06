#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	unsigned int s1, s2;
    char sel;
    asm volatile
        ("movl $0x01 , %%eax ;\n\t"
         "xorl %%edx , %%edx ;\n\t"
         "cpuid ;\n\t"
         "movl %%edx , %0 ;\n\t"
         "movl %%eax , %1 ;\n\t"
         :"=m"(s1), "=m"(s2)
        );

    if (0 == s1 && 0 == s2)
    {
        printf("get cpu id fail\n");
        return -1;
    }

    printf("%08X-%08X\n", htonl(s2), htonl(s1));
}
