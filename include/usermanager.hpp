#pragma once
#include "threadpoll.hpp"
#include "agreement.hpp"
#include <set>
#include <string>
#include <cstring>
#include <queue>


struct userinformation
{ 
        int fd;
        unsigned length;
        char buf[1024];
        unsigned CMD;
};
class usermanager
{
public:
        usermanager(threadpoll* _poll);
        ~usermanager();
        void CREATEusermanager(int _fd ,useragreement _CMD,char *_s);
        void LOGINusermanager(int _fd ,useragreement _CMD,char *_s);
        void LOGOUTusermanager(int _fd ,useragreement _CMD,char *_s);
        unsigned getlength();
        int getfd();
        char* getchar();
        unsigned getCMD();
        unsigned getsize();
        void begin();
        void end();
private:
        userinformation* information;
        std::set<std::string> username;
        threadpoll* poll;
        std::queue<userinformation> usersend;
};  