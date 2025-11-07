#include "recvserver.hpp"
#include "ringbuf.hpp"
#include "message.hpp"
#include <sys/epoll.h>
#include <cstdio>
#include <cerrno>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <iostream>

constexpr int MAX=200;

recvserver::~recvserver()
{
    for(auto &i : clientInfo)
    {
        close(i.first);
    }
    clientInfo.clear();
    close(socket);
    close(epoll_fd);
}

bool recvserver::init(int socket_)
{
    socket=socket_;
    struct epoll_event ev;
    epoll_fd=epoll_create(1);
    if(epoll_fd<0)
    {
        perror("epoll creat error");
        return false;
    }
    ev.events = EPOLLIN | EPOLLRDHUP ;
    ev.data.fd=socket;
    if(epoll_ctl(epoll_fd , EPOLL_CTL_ADD , socket , &ev)<0)
    {
        perror("epoll_ctl error");
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
            if(ready_fd==socket)
            {
                struct sockaddr_in  clientsockaddr;
                socklen_t length =sizeof(clientsockaddr);
                int accept_fd=accept(socket,(sockaddr*)&clientsockaddr , &length);
                recv_buf.try_emplace(accept_fd,ringbuf());
                send_buf.try_emplace(accept_fd,ringbuf());
                clientInfo.try_emplace(accept_fd,std::unique_ptr<client>(new client(clientsockaddr)));
                struct epoll_event es;
                es.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLOUT;
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
                    int length = sizeof(HeadMessage);
                    if(recv_buf[ready_fd].howsize()>length)
                    {                      
                        char buf[length];
                        recv_buf[ready_fd].chack(buf ,length);
                        HeadMessage* Head = (HeadMessage*) buf;
                        if(recv_buf[ready_fd].howsize()>=Head->length_())
                        {
                            if(Head->cmd_()==CMD::CREATE)
                            {
                                char CREATE_buf[Head->length_()];
                                recv_buf[ready_fd].read(CREATE_buf , Head->length_());
                                user.insert(CREATE_buf);
                            }
                            else if(Head->cmd_()==CMD::LOGIN)
                            {
                                char LOGIN_buf[Head->length_()];
                                recv_buf[ready_fd].read(LOGIN_buf , Head->length_());
                                if(user.count(LOGIN_buf))
                                {
                                    LOGIN_RESULT result(1);
                                    send_buf[ready_fd].write((char*)&result, sizeof(result));
                                } 
                                else
                                {
                                    LOGIN_RESULT result(-1);
                                    send_buf[ready_fd].write((char*)&result, sizeof(result));
                                } 
                            }
                            else if(Head->cmd_()==CMD::LOGOUT)
                            {
                                char LOGOUT_buf[Head->length_()];
                                recv_buf[ready_fd].read(LOGOUT_buf , Head->length_());
                                if(user.count(LOGOUT_buf))  
                                {
                                    LOGOUT_RESULT result(1);
                                    send_buf[ready_fd].write((char*)&result, sizeof(result));
                                }
                                else 
                                {
                                    LOGOUT_RESULT result(-1);
                                    send_buf[ready_fd].write((char*)&result, sizeof(result));
                                }
                            }
                            else 
                            {
                                char no_buf[Head->length_()];
                                recv_buf[ready_fd].read(no_buf , Head->length_());     
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
        }
    }
}

