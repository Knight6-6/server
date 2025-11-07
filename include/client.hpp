#pragma once
#include <string>
#include <netinet/in.h>
#include <ctime>

class client
{
public:
       client(sockaddr_in &sockaddr);
       ~client();
       std::string get_ip();
       unsigned short get_port();
       time_t get_time();
private:
       std::string ip;
       unsigned short port;
       time_t client_time;
};