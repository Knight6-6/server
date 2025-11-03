#include "server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>


#define MAX 200

server::server():listensocket(-1),epoll_fd(-1)
{

}
server::~server()
{
    for(auto i : clientInfo)
    {
        close(i.first);
    }
    clientInfo.clear();
    close(listensocket);
    close(epoll_fd);
}
bool server::init(const char* ip , unsigned short port)
{
    listensocket = socket(AF_INET , SOCK_STREAM , 0);
    if(listensocket<0)
    {
        perror("socker creat error");
        return false;
    }
    struct sockaddr_in serversockaddr;
    serversockaddr.sin_port=htons(port);
    serversockaddr.sin_family=AF_INET;
    inet_pton(AF_INET , ip , &serversockaddr.sin_addr);
    int bind_fd=bind(listensocket , (sockaddr*)&serversockaddr , sizeof(serversockaddr));
    if(bind_fd<0)
    {
        perror("bind error");
        return false;
    }
    int listen_fd=listen(listensocket ,128);
    if(listen_fd<0)
    {
        perror("listen error");
        return false;
    }
    return true;
}
bool server::start()
{
    struct epoll_event ev;
    epoll_fd=epoll_create(1);
    if(epoll_fd<0)
    {
        perror("epoll creat error");
        return false;
    }
    ev.events = EPOLLIN | EPOLLRDHUP;
    ev.data.fd=listensocket;
    if(epoll_ctl(epoll_fd , EPOLL_CTL_ADD , listensocket , &ev)<0)
    {
        perror("epoll_ctl error");
        return false;
    }
    while(1)
    {
        struct epoll_event events[MAX];
        int sum=epoll_wait(epoll_fd , events , MAX , 10);
        if(sum<0)
        {
            if(errno==EINTR) continue;
            perror("epoll_wait error");
            return false;
        }
        for(int i=0 ; i<sum ; i++)
        {
            int ready_fd=events[i].data.fd;
            uint32_t type_fd=events[i].events;
            if(ready_fd==listensocket)
            {
                struct sockaddr_in  clientsockaddr;
                socklen_t length =sizeof(clientsockaddr);
                int accept_fd=accept(listensocket , (sockaddr*)&clientsockaddr , &length);
                clientInfo.insert(std::make_pair(accept_fd,client(&clientsockaddr)));
                struct epoll_event es;
                es.events = EPOLLIN;
                es.data.fd=accept_fd;
                if(epoll_ctl(epoll_fd , EPOLL_CTL_ADD , accept_fd , &es)<0)
                {
                    perror("epoll_crl error");
                    return false;
                }
            }
            else 
            {
                if(type_fd&EPOLLRDHUP)
                {
                   std::cout <<"client return";
                   clientInfo.erase(ready_fd);
                   epoll_ctl(epoll_fd , EPOLL_CTL_DEL , ready_fd , NULL);
                } 
                if(type_fd&EPOLLERR)
                {
                    perror("client error");
                }
                if(type_fd&EPOLLIN)
                {
                    int n=recv(ready_fd , serverbuf , sizeof(serverbuf) , 0);
                    if(n>0)
                    {
                        std::cout <<serverbuf<<std::endl;
                    }
                }
                if(type_fd&EPOLLOUT)
                {
                    char clientbuf[7]="hellow";
                    send(ready_fd , clientbuf , sizeof(clientbuf) , 0);
                }
            }
        }
    }
}
