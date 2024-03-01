//
// Created by admin on 2023/9/16.
//

#ifndef HTTP_SERVER_H


#define HTTP_SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <sys/epoll.h>
#include <signal.h>
#define MAXLINE 1024
#define SERV_PORT 8100

typedef struct {
    int sockfd;
    int port;
    char *ip;
    int backlog;
    void (*initSocket)(void);
    void (*bind)(void);
    void (*listen)(void);
} Service;

extern Service service;

extern void initSocket();
extern void Bind();
extern void Listen();


#endif //HTTP_SERVER_H
