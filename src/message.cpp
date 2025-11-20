#include "message.hpp"

HeadMessage::HeadMessage() : cmd(CMD::NO_CMD), length(0) {}
HeadMessage::HeadMessage(CMD _cmd , unsigned _length) : cmd(_cmd) , length(_length){}
unsigned HeadMessage::getlength()
{
    return length;
}
CMD HeadMessage::getcmd()
{
    return cmd;
}


CREATE::CREATE(char* _username , char* _password):HeadMessage(CMD::CREATE , sizeof(CREATE))
{
    strcpy(username , _username);
    strcpy(password , _password);
}

LOGIN::LOGIN(char *_username , char *_password):HeadMessage(CMD::LOGIN , sizeof(LOGIN))
{
    strcpy(username , _username);
    strcpy(password , _password);
}
LOGIN::LOGIN() :HeadMessage(CMD::LOGIN , sizeof(LOGIN)), username{0} ,password{0}{};


LOGOUT::LOGOUT() :HeadMessage(CMD::LOGOUT , sizeof(LOGOUT)){}
LOGOUT::LOGOUT(char *_username):HeadMessage(CMD::LOGOUT , sizeof(LOGOUT))
{
    strcpy(username , _username);
}


LOGIN_RESULT::LOGIN_RESULT() :HeadMessage(CMD::LOGIN_RESULT , sizeof(LOGIN_RESULT)){}
LOGIN_RESULT::LOGIN_RESULT(int _result) :HeadMessage(CMD::LOGIN_RESULT , sizeof(LOGIN_RESULT)), result(_result){}


LOGOUT_RESULT::LOGOUT_RESULT() :HeadMessage(CMD::LOGOUT_RESULT , sizeof(LOGOUT_RESULT)){}
LOGOUT_RESULT::LOGOUT_RESULT(int _result) :HeadMessage(CMD::LOGOUT_RESULT ,sizeof(LOGOUT_RESULT)) ,result(_result){}                 


CREATE_RESULT::CREATE_RESULT() :HeadMessage(CMD::CREATE_RESULT , sizeof(CREATE_RESULT)){}
CREATE_RESULT::CREATE_RESULT(int _result) :HeadMessage(CMD::CREATE_RESULT ,sizeof(CREATE_RESULT)) ,result(_result){}                 
