#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "clientserver/server.h"
#include "clientserver/connection.h"
#include "clientserver/connectionclosedexception.h"

#include <string>

/**
 * Message handler is the low-level message handler for messages to/from the
 * server. It handles transmission/reception of low-level message units.
 */
class MessageHandler {
public:
    
    /**
	 * Create a message handler.
	 * 
	 * @param conn
	 *            The connection to use messages
	 */
    MessageHandler(client_server::Connection* c) : conn(c) { }
    
    void sendCode(unsigned char code);
    
    void sendInt(int value);
    
    void sendIntParameter(int param);
    
    void sendStringParameter(std::string param);
    
    unsigned char recvCode();
    
    int recvInt();
    
    int recvIntParameter();
    
    std::string recvStringParameter();
    
private:
    void sendByte(unsigned char code);
    
    unsigned char recvByte();
    
    client_server::Connection* conn; //The connection

};

#endif