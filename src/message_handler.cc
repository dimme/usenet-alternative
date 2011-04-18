#include "message_handler.h"
#include "clientserver/protocol.h"

#include <iostream>
#include <cstdlib>

using namespace std;
using protocol::Protocol;
using client_server::Server;
using client_server::Connection;
using client_server::ConnectionClosedException;


void MessageHandler::sendByte(int code) {
    try {
        conn->write(static_cast<char>(code));
    } catch (ConnectionClosedException e) {
        throw e;
    }
}
    
void MessageHandler::sendCode(int code) {
    sendByte(code);
}

void MessageHandler::sendInt(int value) {
    sendByte((value >> 24) & 0xFF);
    sendByte((value >> 16) & 0xFF);
    sendByte((value >> 8) & 0xFF);
    sendByte(value & 0xFF);
}

void MessageHandler::sendIntParameter(int param) {
    sendCode(Protocol::PAR_NUM);
    sendInt(param);
}

void MessageHandler::sendStringParameter(std::string param) {
    sendCode(Protocol::PAR_STRING);
    sendInt(param.size());
    for (size_t i = 0; i < param.size(); ++i) {
        sendByte(param[i]);
    }
}


int MessageHandler::recvByte() {
    return conn->read();
}

int MessageHandler::recvCode() {
    return 0;
}

int MessageHandler::recvInt() {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | 
    (byte3 << 8) | byte4;
}

int MessageHandler::recvIntParameter() {
    return 0;
}

std::string MessageHandler::recvStringParameter() {
    return "";
}



