#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "clientserver/server.h"
#include "clientserver/connection.h"
#include "clientserver/connectionclosedexception.h"

#include <string>

class MessageHandler {
public:
    
    MessageHandler(client_server::Connection* c) : conn(c) { }
    
    void sendCode(int code);
    
    void sendInt(int value);
    
    void sendIntParameter(int param);
    
    void sendStringParameter(std::string param);
    
    int recvCode();
    
    int recvInt();
    
    int recvIntParameter();
    
    std::string recvStringParameter();
    
private:
    void sendByte(int code);
    
    int recvByte();
    
    client_server::Connection* conn;

};

#endif