#pragma once 
#include <fstream>
#include <mutex>
#include <string>

class logger 
{
public:
        enum LEVEL
        {
            DEBUG = 0,
            INFO = 1,
            WARN = 2,
            ERROR = 3,
            FATAL = 4
        };
        static logger& getlogger();
        ~logger();
        logger(logger & log)=delete;
        logger& operator=(logger& log)=delete;
        logger(logger&& log)=delete;
        logger& operator=(logger&& log)=delete;
        void debug(std::string& meg);
        void info(std::string& meg);
        void warn(std::string& meg);
        void error(std::string& meg);
        void fatal(std::string& meg);
private:
        logger();
        char file_name[40];
        std::string buf;
        size_t length;
        static constexpr size_t buf_max = 1024 * 1024;
        static constexpr size_t file_max = 1024 *1024 *100;
        std::mutex lock;
        std::ofstream write_file;
        LEVEL current_level = INFO;
};