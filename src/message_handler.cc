#include "message_handler.h"

#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace protocol;
using client_server::Connection;
using client_server::ConnectionClosedException;

/*
 * Private
 */
void MessageHandler::sendByte(unsigned char code) {
    conn->write(code);
}
    
/**
 * Transmit a code (a constant from the Protocol class).
 * 
 * @param code
 *            The code to transmit.
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendCode(unsigned char code) {
    sendByte(code);
    if (!isClient) cout << protocolValue(code) << " ";
    if (Protocol::ANS_END == code && !isClient)
        cout << endl << endl;
}

/**
 * Transmit an int value, according to the protocol.
 * 
 * @param value
 *            The value to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendInt(int value) {
    sendByte((value >> 24) & 0xFF);
    sendByte((value >> 16) & 0xFF);
    sendByte((value >> 8) & 0xFF);
    sendByte(value & 0xFF);
    if (!isClient) cout << value << " ";
}

/**
 * Transmit an int parameter, according to the protocol.
 * 
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendIntParam(int param) {
    sendCode(Protocol::PAR_NUM);
    sendInt(param);
}

/**
 * Transmit a string parameter, according to the protocol.
 * 
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendStringParam(std::string param) {
    sendCode(Protocol::PAR_STRING);
    sendInt(param.size());
    for (size_t i = 0; i < param.size(); ++i) {
        sendByte(param[i]);
    }
    if (!isClient) cout << param << " ";
}

/*
 * Private
 */
unsigned char MessageHandler::recvByte() {
    return conn->read();
}

/**
 * Receive a command code or an error code from the server.
 * 
 * @return The code
 * @throws ConnectionClosedException
 *             If the server died
 */
unsigned char MessageHandler::recvCode() {
    unsigned char code = recvByte();
    if (!isClient) cout << protocolValue(code) << " ";
    if (Protocol::COM_END == code && !isClient)
        cout << endl;
    return code;
}

/**
 * Receive an int value from the server.
 * 
 * @return The int value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvInt() {
    unsigned char byte1 = recvByte();
    unsigned char byte2 = recvByte();
    unsigned char byte3 = recvByte();
    unsigned char byte4 = recvByte();
    int value = byte1 << 24 | byte2 << 16 | byte3 << 8 | byte4;
    if (!isClient) cout << value << " ";
    return value;
}

/**
 * Receive an int parameter from the server.
 * 
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvIntParam() {
    unsigned char code = recvCode();
    if (code != Protocol::PAR_NUM) {
        throw ProtocolViolationException();
    }
    return recvInt();
}

/**
 * Receive a string parameter from the server.
 * 
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */
std::string MessageHandler::recvStringParam() {
    int code = recvCode();
    if (code != Protocol::PAR_STRING) {
        throw ProtocolViolationException();
    }
    int n = recvInt();
    if (n < 0) {
        throw ProtocolViolationException();
    }
    
    stringstream ss;
    
    for (int i = 1; i <= n; ++i) {
        ss << recvByte();
    }
    
    if (!isClient) cout << ss.str() << " ";
    return ss.str();
}




string protocolValue(const int &code) {        
    
    switch (code) {
        case protocol::Protocol::COM_LIST_NG:
            return string("COM_LIST_NG");
            
        case protocol::Protocol::COM_CREATE_NG:
            return string("COM_CREATE_NG");
            
        case protocol::Protocol::COM_DELETE_NG:
            return string("COM_DELETE_NG");
            
        case protocol::Protocol::COM_LIST_ART:
            return string("COM_LIST_ART");
            
        case protocol::Protocol::COM_CREATE_ART:
            return string("COM_CREATE_ART");
            
        case protocol::Protocol::COM_DELETE_ART:
            return string("COM_DELETE_ART");
            
        case protocol::Protocol::COM_GET_ART:
            return string("COM_GET_ART");
            
        case protocol::Protocol::COM_END:
            return string("COM_END");
            
        case protocol::Protocol::ANS_LIST_NG:
            return string("ANS_LIST_NG");
            
        case protocol::Protocol::ANS_CREATE_NG:
            return string("ANS_CREATE_NG");
            
        case protocol::Protocol::ANS_DELETE_NG:
            return string("ANS_DELETE_NG");
            
        case protocol::Protocol::ANS_LIST_ART:
            return string("ANS_LIST_ART");
            
        case protocol::Protocol::ANS_CREATE_ART:
            return string("ANS_CREATE_ART");
            
        case protocol::Protocol::ANS_DELETE_ART:
            return string("ANS_DELETE_ART");
            
        case protocol::Protocol::ANS_GET_ART:
            return string("ANS_GET_ART");
            
        case protocol::Protocol::ANS_END:
            return string("ANS_END");
            
        case protocol::Protocol::ANS_ACK:
            return string("ANS_ACK");
            
        case protocol::Protocol::ANS_NAK:
            return string("ANS_NAK");
            
        case protocol::Protocol::PAR_STRING:
            return string("PAR_STRING");
            
        case protocol::Protocol::PAR_NUM:
            return string("PAR_NUM");
            
        case protocol::Protocol::ERR_NG_ALREADY_EXISTS:
            return string("ERR_NG_ALREADY_EXISTS");
            
        case protocol::Protocol::ERR_NG_DOES_NOT_EXIST:
            return string("ERR_NG_DOES_NOT_EXIST");
            
        case protocol::Protocol::ERR_ART_DOES_NOT_EXIST:
            return string("ERR_ART_DOES_NOT_EXIST");
    }
    return string("Unknown protocol command");
}

string protocolValue(const char &code) {
    return protocolValue(static_cast<int>(code));
}

string protocolValue(const unsigned char &code) {
    return protocolValue(static_cast<int>(code));
}



