#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FIFO_SERVER "/tmp/fifoserver"

int main(int argc, char** argv)
{
    char r_buf[4096*2];
    int fd;
    int r_size;
    int ret_size;
    r_size = atoi(argv[1]);
    printf("requred real read bytes %d\n",r_size);
    memset(r_buf, 0, sizeof(r_buf));
    fd = mkfifo(FIFO_SERVER, O_CREAT | O_EXCL);
    if (fd < 0)
        printf("make fifo failed, %d, %s\n", errno, strerror(errno));
    fd = open(FIFO_SERVER, O_RDONLY | O_NONBLOCK, 0);
    //fd=open(FIFO_SERVER, O_RDONLY,0);
    //在此处可以把读程序编译成两个不同版本：阻塞版本及非阻塞版本
    if (fd == -1)
    {
        printf("open %s for read error,%d,%s\n", FIFO_SERVER, errno, strerror(errno));
        exit(0);    
    }
    while(1)
    {
        memset(r_buf, 0, sizeof(r_buf));
        ret_size=read(fd, r_buf, r_size);
        if(ret_size==-1)
        {
            printf("read failed,%d,%s\n", errno, strerror(errno));
              if(errno==EAGAIN)
                    printf("no data avlaible\n");
        }
        else
            printf("real read bytes %d,%s\n", ret_size, r_buf);
        sleep(1);
    }    
    pause();
    unlink(FIFO_SERVER);
}
