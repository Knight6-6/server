#pragma once
#include <cstring>

enum class CMD
{
    CREATE =1,
    LOGIN  =2,
    LOGOUT =3,
    LOGIN_RESULT   =4,
    LOGOUT_RESULT  =5,
    CREATE_RESULT  =6,
    NO_CMD  =7,
};


class HeadMessage
{
private:
       CMD cmd;
       unsigned length;
public:
       HeadMessage();
       HeadMessage(CMD _cmd , unsigned _length);
       unsigned getlength();
       CMD getcmd();
};


class CREATE:public HeadMessage
{
private:
       char username[32];
       char password[32];
public:
       CREATE(char* _username , char* _password);
};


class LOGIN: public HeadMessage
{
private:
       char username[32];
       char password[32];
public:
       LOGIN(char *_username , char *_password);
       LOGIN();
};


class LOGOUT : public HeadMessage
{
private:
       char username[32];
public:
       LOGOUT();
       LOGOUT(char *_username);
};  


class LOGIN_RESULT : public HeadMessage
{
private:
       int result;
public:
       LOGIN_RESULT();
       LOGIN_RESULT(int _result);
};


class LOGOUT_RESULT : public HeadMessage
{
private:
       int result;
public:
      LOGOUT_RESULT();
      LOGOUT_RESULT(int _result);
};                  


class CREATE_RESULT : public HeadMessage
{
private:
       int result;
public:
      CREATE_RESULT();
      CREATE_RESULT(int _result);
};                  
