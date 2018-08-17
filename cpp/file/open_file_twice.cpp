#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
 
int main()
{
    FILE* fd1 = NULL;
    FILE* fd2 = NULL;
    fd1 = fopen("./hello", "w+");
    fd2 = fopen("./hello", "w+");
    if(NULL == fd1)
    {
        perror("open fd1 failed!\n");
    }
    else
    {
        printf("Success one\n");
        char str1[512] = "write fd1";
        size_t len = fwrite(str1, 1, strlen(str1), fd1);
        printf("len1:%zd\n", len);
        if (len != strlen(str1)) 
            printf("write fd1 failed:%zd,%s!\n", len, strerror(errno));
        else
            printf("write fd1 succeed!\n");
        //fclose(fd1);
    }

    if(NULL == fd2)
    {
        perror("open fd2 failed!\n");
    }
    else
    {
        printf("Success two\n");
        char str2[512] = "write fd2";
        size_t len = fwrite(str2, 1, strlen(str2), fd2);
        printf("len2:%zd\n", len);
        if (len != strlen(str2)) 
            printf("write fd2 failed:%zd,%s!\n", len, strerror(errno));
        else
            printf("write fd2 succeed!\n");
    }

    fclose(fd1);
    fclose(fd2);
    //pause();
    return 0;
}
