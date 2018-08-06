#include <unistd.h>  
#include <signal.h>  
#include <stdio.h>  
#include <stdlib.h> 
//自定义信号处理函数
void my_func(int signum)  
{  
	printf("If you want to quit,please try SIGQUIT\n");  
}  
int main(int argc, char** argv)  
{  
	sigset_t set;
    sigset_t pendset;  
	struct sigaction action1;
    struct sigaction action2;  
	//初始化信号集为空
	if(sigemptyset(&set) < 0)  
        perror("sigemptyset failed\n");  

	//将相应的信号加入信号集
	if (sigaddset(&set, SIGQUIT) < 0)  
		perror("sigaddset failed\n");  

	if (sigaddset(&set, SIGINT) < 0)  
		perror("sigaddset failed\n");  

	//设置信号集屏蔽字
	if(sigprocmask(SIG_BLOCK, &set, NULL) < 0)  
		perror("sigprocmask block failed\n");  
	else
	{  
		printf("sigmask blocked\n");  
		sleep(5);  
	}  
	if(sigprocmask(SIG_UNBLOCK, &set, NULL) < 0)  
		perror("sigprocmask unblock failed\n");  
	else
		printf("sigmask unblock\n");  

	//对相应的信号进行屏蔽处理
	while(1){  
		if (sigismember(&set, SIGINT)){  
			sigemptyset(&action1.sa_mask);  
			action1.sa_handler = my_func;  
			sigaction(SIGINT, &action1, NULL);  
		} else if (sigismember(&set, SIGQUIT)) {  
			sigemptyset(&action2.sa_mask);  
			action2.sa_handler = SIG_DFL;  
			sigaction(SIGTERM, &action2, NULL);  
		}  
	}  
	return 0;
}  
