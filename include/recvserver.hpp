#pragma once 
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <string>
#include "client.hpp"
#include "ringbuf.hpp"
#include "usermanager.hpp"

class recvserver
{
public:
        recvserver(usermanager*user_);
        ~recvserver();
        bool init();
        bool start();
        int EPOLL();
        bool add_clientInfo(int fd ,sockaddr_in &sockaddr);
private:
        int epoll_fd;
        usermanager* user;
        std::unordered_map <int ,ringbuf> recv_buf;
        std::unordered_map <int ,ringbuf> send_buf;
        std::unordered_map <int ,std::unique_ptr<client>> clientInfo;
};
