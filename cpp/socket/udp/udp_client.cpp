//socket client
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd=socket(AF_INET,SOCK_DGRAM, 0);
    char server[128] = "192.168.23.162";
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(27800);
    addr.sin_addr.s_addr = inet_addr(server);

    int len = 0;
    while(1)
    {
        printf("please input：");
        char buf=0;
        scanf("%hhd", &buf);
        len = sendto(sockfd, &buf, sizeof(buf), 0, (struct sockaddr*)&addr, sizeof(addr));
        printf("data send,len=%d\n", len);

        socklen_t addrlen=sizeof(addr);
        len = recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr*)&addr, &addrlen);
        if(len > 0)
        {
            printf("recv data from server=%hhd, len=%d\n", buf, len);
        }
        else
        {
            printf("recv server failed, %d\n", len);
        }
    }
    close(sockfd);
}
