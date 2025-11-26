#include "user_manager.hpp"

usermanager::usermanager(threadpoll* _poll):poll(_poll){}

usermanager::~usermanager(){}

void usermanager::CREATEusermanager(int _fd ,useragreement _CMD,char *_s)
{
    userinformation information;//等待优化
    information.fd=_fd;
    information.CMD=4;
    username.insert(std::string(_s));
    strcpy(information.buf,"账号创建成功");
    information.length=strlen(information.buf);
    usersend.push(information);
}

void usermanager::LOGINusermanager(int _fd ,useragreement _CMD,char *_s)
{
    userinformation information;//等待优化
    information.fd=_fd;
    information.CMD=5;
    if(username.count(std::string(_s)))
    {
        strcpy(information.buf,"登陆成功");
    }
    information.length=strlen(information.buf);
    usersend.push(information);
}

void usermanager::LOGOUTusermanager(int _fd ,useragreement _CMD,char *_s)
{
    userinformation information;//等待优化
    information.fd=_fd;
    information.CMD=6;
    if(username.count(std::string(_s)))
    {
        strcpy(information.buf,"登出成功");
    }
    information.length=strlen(information.buf);
    usersend.push(information);
}

unsigned usermanager::getlength()
{
    return information->length;
}

int usermanager::getfd()
{
    return information->fd;
}

char* usermanager::getchar()
{
    char *s=information->buf;
    return s;
}

unsigned usermanager::getCMD()
{
    return information->CMD;
}

unsigned usermanager::getsize()
{ 
    return usersend.size();
}

void usermanager::begin()
{
    information =&usersend.front();
}

void usermanager::end()
{
    usersend.pop();
    information=nullptr;
}