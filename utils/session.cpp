#include "session.hpp"

session::session(sockaddr_in& so):client_(so){}

void session::init(sockaddr_in&  so)
{
    client_.init(so);
}

void session::clean()
{
    client_.clean();
    sendbuf.clean();
    recvbuf.clean();
}

void session::sendwrite(char* data, size_t len)
{
   sendbuf.write(data,len);
}

void session::sendread(char* dest, size_t len)
{
   sendbuf.read(dest,len);
}

void session::sendchack(char* dest ,size_t len)
{
   sendbuf.chack(dest,len);
}

void session::recvwrite(char* data, size_t len)
{
   recvbuf.write(data,len);
}

void session::recvread(char* dest, size_t len)
{
   recvbuf.read(dest,len);
}

void session::recvchack(char* dest ,size_t len)
{
   recvbuf.chack(dest,len);
}

size_t session::sendhowsize_()
{
   return sendbuf.howsize_();
}

size_t session::recvhowsize_()
{
   return recvbuf.howsize_();
}

size_t session::sendhowsize()
{
   return sendbuf.howsize();
} 

size_t session::recvhowsize()
{
   return recvbuf.howsize();
} 