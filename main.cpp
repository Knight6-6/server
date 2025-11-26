#include "core/server.hpp"
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
        Server.start();
    }
    return 0;
}