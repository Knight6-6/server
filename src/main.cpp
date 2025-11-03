#include "server.hpp"
#include <iostream>

int main()
{
    server Server;
    if(!Server.init("127.0.0.1" , 5050))
    {
        perror("server init error ");
    }
    else 
    {
        if(!Server.start())
        {
            perror("server start error");
        }
    }
    return 0;
}