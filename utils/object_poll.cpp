#include "object_poll.hpp"

objectpoll::objectpoll(){}

objectpoll::~objectpoll(){}

std::unique_ptr<session> objectpoll::acquire(sockaddr_in& s)
{
    std::unique_ptr<session> se;
    if(!object.empty())
    {
        object.front()->init(s);
        se=std::move(object.front());
        object.pop();
    }
    else se=std::make_unique<session>(s);
    return se;
}

void objectpoll::reset(std::unique_ptr<session> se)
{
   se->clean();
   object.push(std::move(se));
}