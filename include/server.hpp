#pragma once
#include <cstring>
#include "client.hpp"
#include <map>

class server
{
public:
        server();
        ~server();
        bool init(const char* ip , unsigned short port);
        bool start();
private:
        int listensocket;
        int epoll_fd;
        char serverbuf[128];
        std::map <int ,client> clientInfo;
};