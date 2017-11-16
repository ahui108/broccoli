#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 27999

int main()
{
	//初始化winsock服务
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	//创建socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(PORT);
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//绑定socket
	bind(sock, (struct sockaddr*)&svraddr, sizeof(svraddr));

	//监听socket
	listen(sock, 5);

	while(1)
	{
		struct sockaddr_in addr;
		int len = sizeof(SOCKADDR);

		char buf[1024] = {0};

		//接受客户端连接
		SOCKET client = accept(sock, (struct sockaddr*)&addr, &len);

		char* ip = inet_ntoa(addr.sin_addr);
		printf("accept client: %s\r\n", ip);

		//接收客户端数据
		if(recv(client, buf, 1024, 0) >0)
		{
			printf("recv client: %s\r\n", buf);

			//向客户端发送数据
			send(client, "hello, client", strlen("hello, client"), 0);
		}
		closesocket(client);
	}

	//关闭socket
	closesocket(sock);

	//关闭winsock服务
	WSACleanup();

	return 0;
}
