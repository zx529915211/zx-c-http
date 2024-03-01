//
// Created by admin on 2023/9/16.
//

#include "server.h"

void initSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        printf("socket create 失败\n");
    }
    service.sockfd = sockfd;
}

void Bind()
{
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // 将字符串形式的IP地址转换为网络字节序的IP地址
    if (inet_pton(AF_INET, service.ip, &(servaddr.sin_addr)) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(service.port);
    int report = 1;
    if(setsockopt(service.sockfd,SOL_SOCKET,SO_REUSEPORT,&report,sizeof(report))!=0){
        printf("设置socket选项失败\n");
    }
    int ret = bind(service.sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret==-1){
        printf("socket bind 失败\n");
    }
}

void Listen()
{
    int ret = listen(service.sockfd, service.backlog);
    if(ret==-1){
        printf("socket listen 失败\n");
    }
}