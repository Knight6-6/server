#pragma once
#include "session.hpp"
#include <netinet/in.h>
#include "queue"
#include <memory>


class objectpoll
{
public:
    objectpoll();
    ~objectpoll();
    std::unique_ptr<session> acquire(sockaddr_in& s);
    void reset(std::unique_ptr<session> se);
private:
    std::queue<std::unique_ptr<session>> object;
};
