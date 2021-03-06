//client.c

int main(int argc, char** argv) 
{ 

    int send_sk; 
    struct sockaddr_in s_addr; 
    socklen_t len = sizeof(s_addr); 
    send_sk = socket(AF_INET, SOCK_STREAM, 0); 
    if(send_sk == -1) 
    { 
        perror("socket failed  "); 
        return -1; 
    } 

    bzero(&s_addr, sizeof(s_addr)); 
    s_addr.sin_family = AF_INET; 
    inet_pton(AF_INET, SER_IP, &s_addr.sin_addr); 
    s_addr.sin_port = htons(SER_PORT); 

    if(connect(send_sk,(struct sockaddr*)&s_addr,len) == -1) 
    { 
        perror("connect fail  "); 
        return -1; 
    } 

    char pcContent[5000]={0};
    write(send_sk,pcContent,5000);
    sleep(1);
    close(send_sk);
}
