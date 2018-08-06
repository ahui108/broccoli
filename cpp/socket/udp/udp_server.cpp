//socket server
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
    int sockfd=socket(AF_INET, SOCK_DGRAM, 0);
    //char local_server_ip[128] = "192.168.23.162"; 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(27800);
    //addr.sin_addr.s_addr = inet_addr(local_server_ip);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret =bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(0 > ret)
    {
        printf("socket bind failed\n");
        return -1;
    }
    else
    {
        printf("socket bind succeed\n");
    }
    int data_len = 0;
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    while(1)
    {
        printf("receving data...\n");
        char buf = 0;
        data_len = recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr*)&cli, &len);
        if (len > 0)
            printf("recv data from client=%hhd, len=%d\n", buf, data_len);
        else
            printf("recv data form client failed!\n");

        buf = 66;
        data_len = sendto(sockfd, &buf, sizeof(buf), 0, (struct sockaddr*)&cli, len);
        printf("send data to client=%hhd, len=%d\n", buf, data_len);
    }
    close(sockfd);
}
