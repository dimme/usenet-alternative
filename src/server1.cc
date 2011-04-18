#include <iostream>
#include "message_handler.h"
#include "clientserver/protocol.h"


using namespace std;
using protocol::Protocol;
using protocol::ProtocolViolationException;
using client_server::Server;
using client_server::Connection;
using client_server::ConnectionClosedException;


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << endl;
        exit(1);
    }
    
    Server server(atoi(argv[1]));
    if (! server.isReady()) {
        cerr << "Server initialization error" << endl;
        exit(1);
    }
    
    
    while (true) {
        Connection* conn = server.waitForActivity();
        if (conn != 0) {
            try {
                
                MessageHandler handler(conn);
                unsigned char code = handler.recvCode();
                
                switch (code) {
                    case Protocol::COM_LIST_NG:
                        break;
                        
                    case Protocol::COM_CREATE_NG:
                        break;
                        
                    case Protocol::COM_DELETE_NG:
                        break;
                        
                    case Protocol::COM_LIST_ART:
                        break;
                        
                    case Protocol::COM_CREATE_ART:
                        break;
                        
                    case Protocol::COM_DELETE_ART:
                        break;
                        
                    case Protocol::COM_GET_ART:
                        break;
                        
                    case Protocol::COM_END:
                        break;

                    default:
                        break;
                }
                
                
                
                
                
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                delete conn;
                cout << "Client closed connection" << endl;
            } catch (ProtocolViolationException&) {
                cout << "The protocol was violated by the client" << endl;
            }
        } else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
    }
}