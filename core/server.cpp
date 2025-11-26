#include "server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <thread>


constexpr unsigned IO=20; 

server::server():listensocket(-1),globalpoll(20){}

server::~server(){}

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
    for(int i=0 ; i<IO ; i++)
    {
        usermanager_.emplace_back(std::make_unique<usermanager>(&globalpoll));
        objectpoll_.emplace_back(std::make_unique<objectpoll>());
    }
    for(int i=0 ; i<IO ; i++)
    {
        IOthread.emplace_back(std::make_unique<recvserver>(usermanager_[i].get() , objectpoll_[i].get()));
    }    
    for(int i=0 ; i<IO ; i++)
    {
        std::thread([this,i]()
       {
           if(!IOthread[i]->init())
           {
              return;
           }
          IOthread[i]->start();
       }).detach();
    }
    int nextIO=0;
    while(true)
    {
        struct sockaddr_in clientbuf;
        socklen_t length=sizeof(clientbuf);
        int acceptfd=accept(listensocket ,(sockaddr*)&clientbuf , &length);
        if(acceptfd<0)
        { 
            perror("accept error");
            return false;  
        }
        int s= nextIO++ % IO;
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLERR| EPOLLOUT |EPOLLRDHUP ;
        ev.data.fd =acceptfd;
        epoll_ctl(IOthread[s]->EPOLL(),EPOLL_CTL_ADD , acceptfd , &ev);
        IOthread[s]->add_clientInfo(acceptfd,clientbuf);
    }
}
