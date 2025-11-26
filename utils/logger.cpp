#include "logger.hpp"
#include <ctime>
#include <sys/stat.h>
#include <cstring>


void logger::debug(std::string& meg)
{
    if(DEBUG<current_level)
    {
        return;
    }
    std::lock_guard<std::mutex> lock_(lock);
    time_t t=time(nullptr);
    struct tm* time_=localtime(&t);
    char time_buf[40],time_name[40];
    strftime(time_buf , sizeof(time_buf) ,"[%Y-%m-%d-%H:%M:%S]" ,time_);
    strftime(time_name , sizeof(time_name) ,"log_%Y-%m-%d-%H_%M_%S.txt",time_);
    buf+=time_buf;
    buf+="[DEBUG]";
    buf+=(meg+"\n");
    if(buf.size()>=buf_max)
    { 
        if(length>=file_max)
        {
           write_file.close();
           strcpy(file_name ,time_name);
           write_file.open(file_name,std::ios::out|std::ios::app);
           length=0;
        }
        write_file <<buf;
        length+=buf.size();
        buf.clear();     
    }
}

void logger::info(std::string& meg)
{
    if(INFO<current_level)
    {
        return;
    }
    std::lock_guard<std::mutex> lock_(lock);
    time_t t=time(nullptr);
    struct tm* time_=localtime(&t);
    char time_buf[40],time_name[40];
    strftime(time_buf , sizeof(time_buf) ,"[%Y-%m-%d-%H:%M:%S]" ,time_);
    strftime(time_name , sizeof(time_name) ,"log_%Y-%m-%d-%H_%M_%S.txt",time_);
    buf+=time_buf;
    buf+="[INFO]";
    buf+=(meg+"\n");
    if(buf.size()>=buf_max)
    { 
        if(length>=file_max)
        {
           write_file.close();
           strcpy(file_name ,time_name);
           write_file.open(file_name,std::ios::out|std::ios::app);
           length=0;
        }
        write_file <<buf;
        length+=buf.size();
        buf.clear();     
    }
}

void logger::warn(std::string& meg)
{
   if(WARN<current_level)
    {
        return;
    }
    std::lock_guard<std::mutex> lock_(lock);
    time_t t=time(nullptr);
    struct tm* time_=localtime(&t);
    char time_buf[40],time_name[40];
    strftime(time_buf , sizeof(time_buf) ,"[%Y-%m-%d-%H:%M:%S]" ,time_);
    strftime(time_name , sizeof(time_name) ,"log_%Y-%m-%d-%H_%M_%S.txt",time_);
    buf+=time_buf;
    buf+="[WARN]";
    buf+=(meg+"\n");
    if(buf.size()>=buf_max)
    { 
        if(length>=file_max)
        {
           write_file.close();
           strcpy(file_name ,time_name);
           write_file.open(file_name,std::ios::out|std::ios::app);
           length=0;
        }
        write_file <<buf;
        length+=buf.size();
        buf.clear();     
    }
}

void logger::error(std::string& meg)
{
   if(ERROR<current_level)
    {
        return;
    }
    std::lock_guard<std::mutex> lock_(lock);
    time_t t=time(nullptr);
    struct tm* time_=localtime(&t);
    char time_buf[40],time_name[40];
    strftime(time_buf , sizeof(time_buf) ,"[%Y-%m-%d-%H:%M:%S]" ,time_);
    strftime(time_name , sizeof(time_name) ,"log_%Y-%m-%d-%H_%M_%S.txt",time_);
    buf+=time_buf;
    buf+="[ERROR]";
    buf+=(meg+"\n");
    if(length>=file_max)
    {
        write_file.close();
        strcpy(file_name ,time_name);
        write_file.open(file_name,std::ios::out|std::ios::app);
        length=0;
    }
    write_file <<buf;
    length+=buf.size();
    buf.clear();     
}

void logger::fatal(std::string& meg)
{
    if(FATAL<current_level)
    {
        return;
    }
    std::lock_guard<std::mutex> lock_(lock);
    time_t t=time(nullptr);
    struct tm* time_=localtime(&t);
    char time_buf[40],time_name[40];
    strftime(time_buf , sizeof(time_buf) ,"[%Y-%m-%d-%H:%M:%S]" ,time_);
    strftime(time_name , sizeof(time_name) ,"log_%Y-%m-%d-%H_%M_%S.txt",time_);
    buf+=time_buf;
    buf+="[FATAL]";
    buf+=(meg+"\n");
    if(length>=file_max)
    {
        write_file.close();
        strcpy(file_name ,time_name);
        write_file.open(file_name,std::ios::out|std::ios::app);
        length=0;
    }
    write_file <<buf;
    length+=buf.size();
    buf.clear();     
}

logger& logger::getlogger()
{
    static logger lg;
    return lg;
}

logger::logger()
{
    strcpy(file_name,"FIRST file");
    write_file.open(file_name,std::ios::out|std::ios::app);
}