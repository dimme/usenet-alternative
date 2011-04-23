#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "clientserver/server.h"
#include "clientserver/connection.h"
#include "clientserver/connectionclosedexception.h"
#include "clientserver/protocol.h"

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
     * @param ic
     *            Whether the MessageHandler is used by a client or not
	 */
    MessageHandler(client_server::Connection* c) : conn(c), isClient(false) { }
    MessageHandler(client_server::Connection* c, bool ic) : conn(c), isClient(ic) { }
    
    /*
     * Copy constructor
     */
    MessageHandler(const MessageHandler& h) : conn(h.conn), isClient(h.isClient) {}
    
    void sendCode(unsigned char code);
    
    void sendInt(int value);
    
    void sendIntParam(int param);
    
    void sendStringParam(std::string param);
    
    unsigned char recvCode();
    
    int recvInt();
    
    int recvIntParam();
    
    std::string recvStringParam();
    
private:
    void sendByte(unsigned char code);
    
    unsigned char recvByte();
    
    client_server::Connection* conn; //The connection
    
    bool isClient;
};


std::string protocolValue(const int &code);
std::string protocolValue(const char &code);
std::string protocolValue(const unsigned char &code);


#endif
