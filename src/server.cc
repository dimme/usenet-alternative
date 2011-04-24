#include <iostream>
#include <cstdlib>
#include "message_handler.h"
#include "clientserver/protocol.h"
#include "database.h"


using namespace std;
using protocol::Protocol;
using protocol::ProtocolViolationException;
using client_server::Server;
using client_server::Connection;
using client_server::ConnectionClosedException;

/*
 * Listing all newsgroups
 */
void listNg(MessageHandler& handler, Database& db) {    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    
    GroupList list = db.listNewsgroups();
    
    cout << "Sending: ";
    handler.sendCode(Protocol::ANS_LIST_NG);
    handler.sendIntParam(list.size());
    
    for (GroupList::iterator it = list.begin(); it != list.end(); ++it) {
        handler.sendIntParam(it->first.ident);
        handler.sendStringParam(it->first.name);
    }
    
    handler.sendCode(Protocol::ANS_END);
}

/*
 * Creating a newsgroup
 */
void createNg(MessageHandler& handler, Database& db) {    
    string title = handler.recvStringParam();
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    
    cout << "Sending: ";
    handler.sendCode(Protocol::ANS_CREATE_NG);

    try {
        db.addNewsgroup(title);
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupAlreadyExistsException&) {
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
    }
    
    handler.sendCode(Protocol::ANS_END);
}

/*
 * Deleting a newsgroup
 */
void deleteNg(MessageHandler& handler, Database& db) {    
    int groupID = handler.recvIntParam();
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    
    cout << "Sending: ";
    handler.sendCode(Protocol::ANS_DELETE_NG);
    
    try {
        db.deleteNewsgroup(groupID);
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupDoesNotExistException&) {
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    
    handler.sendCode(Protocol::ANS_END);
}

/*
 * Listing all articles for a newsgroup
 */
void listArt(MessageHandler& handler, Database& db) {    
    int groupID = handler.recvIntParam();
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    
    cout << "Sending: ";
    handler.sendCode(Protocol::ANS_LIST_ART);
    
    try {
        ArticleList list = db.listArticles(groupID);
        
        handler.sendCode(Protocol::ANS_ACK);
        handler.sendIntParam(list.size());
        
        for(ArticleList::iterator it = list.begin(); it != list.end(); ++it) {
			handler.sendIntParam(it->ident);
			handler.sendStringParam(it->title);
		}
    } catch (NewsgroupDoesNotExistException&) {
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    
    handler.sendCode(Protocol::ANS_END);
}

/*
 * Creating an article in a newsgroup
 */
void createArt(MessageHandler& handler, Database& db) {    
    int groupID = handler.recvIntParam();
    
    string title = handler.recvStringParam();
    
    string authorStr = handler.recvStringParam();
    User author(authorStr);
    
    string content = handler.recvStringParam();
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    
    cout << "Sending: ";
    handler.sendCode(Protocol::ANS_CREATE_ART);
    
    try {
        db.addArticle(groupID, author, title, content);
        
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupDoesNotExistException&) {
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    
    handler.sendCode(Protocol::ANS_END);
}

/*
 * Deleting an article in a newsgroup
 */
void deleteArt(MessageHandler& handler, Database& db) {    
    int groupID = handler.recvIntParam();
    
    int artID = handler.recvIntParam();
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    
    cout << "Sending: ";
    handler.sendCode(Protocol::ANS_DELETE_ART);
    
    try {
        db.deleteArticle(groupID, artID);
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupDoesNotExistException&) {
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } catch (ArticleDoesNotExistException&) {
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    
    handler.sendCode(Protocol::ANS_END);
}

/*
 * Getting an article from a newsgroup
 */
void getArt(MessageHandler& handler, Database& db) {    
    int groupID = handler.recvIntParam();
    
    int artID = handler.recvIntParam();
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    
    cout << "Sending: ";
    handler.sendCode(Protocol::ANS_GET_ART);
    
    try {
        Article art = db.getArticle(groupID, artID);
        
        handler.sendCode(Protocol::ANS_ACK);
        handler.sendStringParam(art.title);
        handler.sendStringParam(art.author.name);
        handler.sendStringParam(art.content);
        
    } catch (NewsgroupDoesNotExistException&) {
        
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
        
    } catch (ArticleDoesNotExistException&) {
        
        handler.sendCode(Protocol::ANS_NAK);
        handler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    
    handler.sendCode(Protocol::ANS_END);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/*
 * The main
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: server_[mem|disk] port-number" << endl;
        exit(1);
    }
    
    Server server(atoi(argv[1]));
    if (! server.isReady()) {
        cerr << "Server initialization error" << endl;
        exit(1);
    }
    
    Database db;
    
    while (true) {
        Connection* conn = server.waitForActivity();
        if (conn != 0) {
            try {
                
                MessageHandler handler(conn);
                
                cout << "Receiving: ";
                unsigned char code = handler.recvCode();
                
                switch (code) {
                    case Protocol::COM_LIST_NG:
                        listNg(handler, db);
                        break;
                        
                    case Protocol::COM_CREATE_NG:
                        createNg(handler, db);
                        break;
                        
                    case Protocol::COM_DELETE_NG:
                        deleteNg(handler, db);
                        break;
                        
                    case Protocol::COM_LIST_ART:
                        listArt(handler, db);
                        break;
                        
                    case Protocol::COM_CREATE_ART:
                        createArt(handler, db);
                        break;
                        
                    case Protocol::COM_DELETE_ART:
                        deleteArt(handler, db);
                        break;
                        
                    case Protocol::COM_GET_ART:
                        getArt(handler, db);
                        break;
                        
                    default:
                        throw ProtocolViolationException();
                        break;
                }
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                delete conn;
                cout << "Client closed connection" << endl;
            } catch (ProtocolViolationException&) {
                cout << "The protocol was violated by the client" << endl;
                server.deregisterConnection(conn);
                delete conn;
                cout << "Connection closed" << endl;
            }
            
        } else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
    }
}
