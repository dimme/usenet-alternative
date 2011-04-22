#include "message_handler.h"

#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;
using protocol::Protocol;
using protocol::ProtocolViolationException;
using client_server::Connection;
using client_server::ConnectionClosedException;

/*
 * Private
 */
void MessageHandler::sendByte(unsigned char code) {
    conn->write(code);
    cout << "Sent byte: " << static_cast<int>(code) << endl;;
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
    return recvByte();
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
    return byte1 << 24 | byte2 << 16 | byte3 << 8 | byte4;
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
    return ss.str();
}



