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
                // Do whatever
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                delete conn;
                cout << "Client closed connection" << endl;
            }
        } else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
    }
}