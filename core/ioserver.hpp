#pragma once 
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <string>
#include "object_poll.hpp"
#include "user_manager.hpp"

class recvserver
{
public:
        recvserver(usermanager*user_ , objectpoll* object_);
        ~recvserver();
        bool init();
        bool start();
        int EPOLL();
        bool add_clientInfo(int fd ,sockaddr_in &sockaddr);
private:
        int epoll_fd;
        usermanager* user;
        objectpoll* object;
        std::unordered_map <int ,std::unique_ptr<session>> clientInfo;
};
