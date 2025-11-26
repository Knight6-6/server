#pragma once
#include <cstring>
#include <vector>
#include "thread_poll.hpp"
#include "user_manager.hpp"
#include "ioserver.hpp"
#include "object_poll.hpp"


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
        std::vector<std::unique_ptr<objectpoll>> objectpoll_;
        std::vector<std::unique_ptr<usermanager>> usermanager_;
        std::vector<std::unique_ptr<recvserver>> IOthread;
};