#include "server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "recvserver.hpp"


server::server():listensocket(-1){}

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
    recvserver recv_server;
    if(!recv_server.init(listensocket))
    {
        return false;
        perror("reveserver init error ");
    }
    return recv_server.start();
}
