#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
int main ()
{
    printf("begin\n");
    printf("pid=%d\n", getpid());
    int count=0;
    pid_t pid;
    pid=fork();
    if (pid < 0)
        printf("error in fork!");
    else if (pid == 0) {
        printf("pid=%d\n",getpid());
        printf("son\n");
        count++;
        sleep(10);
    } else {
        printf("pid=%d\n",getpid());
        printf("father\n");
        //usleep(100);
        //sleep(10);
        count++;
    }
    //wait(&pid);
    printf("count:%d\n",count);
    return 0;
}
