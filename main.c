#include "server.h"

Service service = {
        .ip="0.0.0.0",
        .port=SERV_PORT,
        .backlog=1024,
        .initSocket=initSocket,
        .bind=Bind,
        .listen=Listen,
};

int main() {
    struct sockaddr_in  cliaddr;
    socklen_t cliaddr_len;
    int connfd;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;


    service.initSocket();
//    listenfd = socket(AF_INET, SOCK_STREAM, 0);

//    bzero(&servaddr, sizeof(servaddr));
//    servaddr.sin_family = AF_INET;
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    servaddr.sin_port = htons(SERV_PORT);
//
//    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    service.bind();
    service.listen();
//    listen(listenfd, 20);

    int epfd = epoll_create(1024);
    if(epfd==-1){
        printf("epoll create fail\n");
        exit(0);
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = service.sockfd;

    int res = epoll_ctl(epfd,EPOLL_CTL_ADD,service.sockfd,&event);

    if(res==-1){
        perror("epoll_ctl fail:");
    }

    struct epoll_event events[1024];

    while (1){
        //timeout=-1 阻塞调用
        //timeout=0  非阻塞调用
        int ret = epoll_wait(epfd,events,1024,-1);
        if(ret==-1){
            if(errno==EINTR){
                //有中断信号来了，继续执行
                continue;
            } else{
                break;
            }
        }
        if(ret>0){
            for (int i = 0; i < ret; ++i) {
                int fd = events[i].data.fd;
                if(events[i].events == EPOLLIN){
                    if(fd == service.sockfd){
                        //监听socket
                        cliaddr_len = sizeof(cliaddr);
                        connfd = accept(service.sockfd,(struct sockaddr *)&cliaddr, &cliaddr_len);
                        printf("Accepting connections ...\n");
                        struct epoll_event event;
                        event.events = EPOLLIN;
                        event.data.fd = connfd;
                        ret = epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&event);
                    }else{
                        n = read(connfd, buf, MAXLINE);
                        if(n>0){
                            for (i = 0; i < n; i++)
                                buf[i] = toupper(buf[i]);
                            write(fd, buf, n);
                        } else if(n<=0){
                            close(fd);
                            epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                        }
                    }
                }
            }
        }
    }

//    while (1) {
//
//        printf("received from %s at PORT %d\n",
//               inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
//               ntohs(cliaddr.sin_port));
//
//        for (i = 0; i < n; i++)
//            buf[i] = toupper(buf[i]);
//        write(connfd, buf, n);
//        close(connfd);
//    }
}
