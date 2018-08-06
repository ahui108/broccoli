#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
static void sig_user(int signum)
{
    printf("signum:%d\n", signum);
    if (signum == SIGPIPE)
        printf("SIGPIPE\n");
}

int main(int argc, char** argv)
{
      int pipe_fd[2];
      pid_t pid;
      char r_buf[256];
      char w_buf[4096*16];
      char* p_wbuf;
      int r_num;
      int w_num;
      int cmd;
      signal(SIGPIPE, sig_user);
      memset(r_buf, 0, sizeof(r_buf));
      memset(w_buf, 0, sizeof(r_buf));
      p_wbuf=w_buf;

      if(pipe(pipe_fd)<0)
      {
            printf("pipe create error\n");
            return -1;
      }
      if((pid=fork()) == 0)
      {
            close(pipe_fd[1]); //close fd[1], read from fd[0]
            //usleep(100);
            do
            {
                r_num=read(pipe_fd[0], r_buf, 256);
                if (r_num < 0)
                {
                    printf("pipe read failed,%d,%s\n", errno, strerror(errno));
                    //usleep(100);
                }
                else
                {
                    printf("read num=%d, data read from pipe:%s\n", r_num, r_buf);
                }
            }while(r_num);
            close(pipe_fd[0]);
            printf("child close fd[0]\n");
            exit(0);
      }
      else if(pid > 0)
      {
           //sleep(2);
           close(pipe_fd[0]); //close fd[0], write to fd[1]
           memset(w_buf, 0, sizeof(w_buf));
           strcpy(w_buf, "##message from parent##");
           int index = 0; 
           while(index < sizeof(w_buf) && (w_num = write(pipe_fd[1], w_buf, 1024)) != -1)
           {
               if (w_num < 0)
                   printf("parent write failed,%d,%s\n", errno, strerror(errno));
               else
               {
                   index += w_num;
                   //r_num=read(pipe_fd[0], r_buf, 256);
                   //printf("parent read:%d,%s\n", r_num, r_buf);
                   printf("parent write over %d\n", w_num);
               }    
           }    
           close(pipe_fd[1]);
           printf("parent close fd[1]\n");
           sleep(3);
      }
      wait(&pid);

    return 0;
}
