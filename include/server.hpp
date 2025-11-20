#pragma once
#include <cstring>
#include <vector>
#include "threadpoll.hpp"
#include "usermanager.hpp"
#include "recvserver.hpp"


class server
{
public:
        server();
        ~server();
        bool init(const char* ip , unsigned short port);
        bool start();
private:
        int listensocket;
        threadpoll globalpoll;
        std::vector<std::unique_ptr<usermanager>> usermanager_;
        std::vector<std::unique_ptr<recvserver>> IOthread;
};