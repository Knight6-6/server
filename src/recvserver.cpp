#include "recvserver.hpp"
#include "ringbuf.hpp"
#include "message.hpp"
#include "agreement.hpp"
#include <sys/epoll.h>
#include <cstdio>
#include <cerrno>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <iostream>


constexpr int MAX=200;

recvserver::recvserver(usermanager* user_):user(user_){}

recvserver::~recvserver()
{
    for(auto &i : clientInfo)
    {
        close(i.first);
    }
    clientInfo.clear();
    close(epoll_fd);
}

int recvserver::EPOLL()
{
    return epoll_fd;
}

bool recvserver::add_clientInfo(int fd ,sockaddr_in &sockaddr)
{
    clientInfo.try_emplace(fd,std::make_unique<client>(sockaddr));
    return true;
}

bool recvserver::init()
{
    struct epoll_event ev;
    epoll_fd=epoll_create(1);
    if(epoll_fd<0)
    {
        perror("epoll creat error");
        return false;
    }
    return true;
}

bool recvserver::start() 
{
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
            if(type_fd&EPOLLRDHUP)
            {
                std::cout <<"client return";
                clientInfo.erase(ready_fd);
                recv_buf.erase(ready_fd);
                send_buf.erase(ready_fd);
                epoll_ctl(epoll_fd , EPOLL_CTL_DEL , ready_fd , NULL);
            } 
            if(type_fd&EPOLLERR)
            {
                perror("client error");
            }
            if(type_fd&EPOLLIN)
            {
                int len=recv_buf[ready_fd].howsize_();
                if(len>0)
                {
                    char buf[1024];
                    int sum=recv(ready_fd , buf , len , 0);
                    if(sum>0)
                    {
                        recv_buf[ready_fd].write(buf , sum);
                    }    
                } 
                unsigned length = sizeof(HeadMessage);
                if(recv_buf[ready_fd].howsize()>length)
                {                      
                    char buf[length];
                    recv_buf[ready_fd].chack(buf ,length);
                    HeadMessage* Head = (HeadMessage*) buf;
                    if(recv_buf[ready_fd].howsize()>=Head->getlength())
                    {
                        CMD type=Head->getcmd();
                        char CREATE_buf[Head->getlength()];
                        recv_buf[ready_fd].read(CREATE_buf , Head->getlength());
                        char* s=CREATE_buf+length;
                        switch(type)
                        {
                            case CMD::CREATE: 
                               user->CREATEusermanager(ready_fd ,useragreement::CREATE,s);
                               break;                     
                            case CMD::LOGIN:
                               user->LOGINusermanager(ready_fd,useragreement::LOGIN,s);
                               break;
                            case CMD::LOGOUT:
                               user->LOGOUTusermanager(ready_fd,useragreement::LOGOUT,s);
                               break;
                        } 
                    }
                }                    
            }
            if(type_fd&EPOLLOUT)
            {
                size_t leng=send_buf[ready_fd].howsize();
                if(leng>0)
                {
                    char buf[leng];
                    send_buf[ready_fd].read(buf , leng);
                    send(ready_fd , buf , leng , 0 );
                }
            }
        }
        int l=user->getsize();
        if(l>0)
        {
            user->begin();
            unsigned size=sizeof(HeadMessage);
            for(int i=0 ; i<l ; i++)
            {
                unsigned len=user->getlength()+size;
                int fd=user->getfd();
                if(len<=send_buf[fd].howsize_())
                {
                    unsigned cmd=user->getCMD();
                    HeadMessage Head((CMD)cmd,len);
                    send_buf[fd].write((char*)&Head,size);
                    send_buf[fd].write(user->getchar(),len-size);
                    user->end();
                }
            }
        }
    }
}