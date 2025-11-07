#pragma once 
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <string>
#include "client.hpp"
#include "ringbuf.hpp"

class recvserver
{
public:
        ~recvserver();
        bool init(int socket_);
        bool start();
private:
        int socket;
        int epoll_fd;
        std::unordered_set <std::string> user;
        std::unordered_map <int ,ringbuf> recv_buf;
        std::unordered_map <int ,ringbuf> send_buf;
        std::unordered_map <int ,std::unique_ptr<client>> clientInfo;
};
