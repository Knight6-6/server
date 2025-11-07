#pragma once
#include <cstring>

class server
{
public:
        server();
        ~server();
        bool init(const char* ip , unsigned short port);
        bool start();
private:
        int listensocket;
};