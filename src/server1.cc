#include <iostream>
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
    cout << "=== Start: Listing newsgroups ===" << endl;
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    cout << "Received: COM_END" << endl;
    
    GroupList list = db.listNewsgroups();
    
    cout << "Sending: ANS_LIST_NG" << endl;
    handler.sendCode(Protocol::ANS_LIST_NG);
    
    cout << "Sending: " << list.size() << endl;
    handler.sendInt(list.size());
    
    
    for (GroupList::iterator it = list.begin(); it != list.end(); ++it) {
        cout << "Sending: " << it->first.ident << ", " << it->first.name << endl;
        handler.sendIntParam(it->first.ident);
        handler.sendStringParam(it->first.name);
    }
    
    cout << "Sending: ANS_END" << endl;
    handler.sendCode(Protocol::ANS_END);
    
    cout << "=== End: Listing newsgroups ===" << endl << endl;
}

/*
 * Creating a newsgroup
 */
void createNg(MessageHandler& handler, Database& db) {
    cout << "=== Start: Creating newsgroup ===" << endl;
    
    string title = handler.recvStringParam();
    cout << "Received: " << title << endl;
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    cout << "Received: COM_END" << endl;
    
    
    try {
        db.addNewsgroup(title);
        cout << "Sending: ANS_ACK" << endl;
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupAlreadyExistsException&) {
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_NG_ALREADY_EXISTS" << endl;
        handler.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
    }
    
    cout << "Sending: ANS_END" << endl;
    handler.sendCode(Protocol::ANS_END);
    
    cout << "=== End: Creating newsgroup ===" << endl << endl;
}

/*
 * Deleting a newsgroup
 */
void deleteNg(MessageHandler& handler, Database& db) {
    cout << "=== Start: Deleting newsgroup ===" << endl;
    
    int groupID = handler.recvIntParam();
    cout << "Received: " << groupID << endl;

    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    cout << "Received: COM_END" << endl;
    
    cout << "Sending: ANS_DELETE_NG" << endl;
    handler.sendCode(Protocol::ANS_DELETE_NG);
    
    try {
        db.deleteNewsgroup(groupID);
        cout << "Sending: ANS_ACK" << endl;
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupDoesNotExistException&) {
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_NG_DOES_NOT_EXIST" << endl;
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    
    cout << "Sending: ANS_END" << endl;
    handler.sendCode(Protocol::ANS_END);
    
    cout << "=== End: Deleting newsgroup ===" << endl << endl;
}

/*
 * Listing all articles for a newsgroup
 */
void listArt(MessageHandler& handler, Database& db) {
    cout << "=== Start: Listing articles ===" << endl;
    
    int groupID = handler.recvIntParam();
    cout << "Received: " << groupID << endl;
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    cout << "Received: COM_END" << endl;
    
    cout << "Sending: ANS_LIST_ART" << endl;
    handler.sendCode(Protocol::ANS_LIST_ART);
    
    try {
        ArticleList list = db.listArticles(groupID);
        
        cout << "Sending: ANS_ACK" << endl;
        handler.sendCode(Protocol::ANS_ACK);
        
        cout << "Sending: " << list.size() << endl;
        handler.sendIntParam(list.size());
        
        for(ArticleList::iterator it = list.begin(); it != list.end(); ++it) {
            cout << "Sending: " << it->ident << ", " << it->title << endl;
			handler.sendIntParam(it->ident);
			handler.sendStringParam(it->title);
		}
    } catch (NewsgroupDoesNotExistException&) {
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_NG_DOES_NOT_EXIST" << endl;
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    
    cout << "Sending: ANS_END" << endl;
    handler.sendCode(Protocol::ANS_END);
    
    cout << "=== End: Listing articles ===" << endl << endl;
}

/*
 * Creating an article in a newsgroup
 */
void createArt(MessageHandler& handler, Database& db) {
    cout << "=== Start: Creating article ===" << endl;
    
    int groupID = handler.recvIntParam();
    cout << "Received: " << groupID << endl;
    
    string title = handler.recvStringParam();
    cout << "Received: " << title << endl;
    
    string authorStr = handler.recvStringParam();
    User author(authorStr);
    cout << "Received: " << authorStr << endl;
    
    string content = handler.recvStringParam();
    cout << "Received: " << content << endl;
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    cout << "Received: COM_END" << endl;
    
    cout << "Sending: ANS_CREATE_ART" << endl;
    handler.sendCode(Protocol::ANS_CREATE_ART);
    
    try {
        db.addArticle(groupID, author, title, content);
        
        cout << "Sending: ANS_ACK" << endl;
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupDoesNotExistException&) {
        
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_NG_DOES_NOT_EXIST" << endl;
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    
    cout << "Sending: ANS_END" << endl;
    handler.sendCode(Protocol::ANS_END);
    
    cout << "=== End: Creating article ===" << endl << endl;
}

/*
 * Deleting an article in a newsgroup
 */
void deleteArt(MessageHandler& handler, Database& db) {
    cout << "=== Start: Deleting article ===" << endl;
    
    int groupID = handler.recvIntParam();
    cout << "Received: " << groupID << endl;
    
    int artID = handler.recvIntParam();
    cout << "Received: " << artID << endl;
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    cout << "Received: COM_END" << endl;
    
    
    cout << "Sending: ANS_DELETE_ART" << endl;
    handler.sendCode(Protocol::ANS_DELETE_ART);
    
    try {
        db.deleteArticle(groupID, artID);
        
        cout << "Sending: ANS_ACK" << endl;
        handler.sendCode(Protocol::ANS_ACK);
    } catch (NewsgroupDoesNotExistException&) {
        
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_NG_DOES_NOT_EXIST" << endl;
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } catch (ArticleDoesNotExistException&) {
        
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_ART_DOES_NOT_EXIST" << endl;
        handler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    
    cout << "Sending: ANS_END" << endl;
    handler.sendCode(Protocol::ANS_END);
    
    cout << "=== End: Deleting article ===" << endl << endl;
}

/*
 * Getting an article from a newsgroup
 */
void getArt(MessageHandler& handler, Database& db) {
    cout << "=== Start: Getting article ===" << endl;
    
    int groupID = handler.recvIntParam();
    cout << "Received: " << groupID << endl;
    
    int artID = handler.recvIntParam();
    cout << "Received: " << artID << endl;
    
    if(handler.recvCode() != Protocol::COM_END) {
        throw ProtocolViolationException();
    }
    cout << "Received: COM_END" << endl;
    
    
    cout << "Sending: ANS_GET_ART" << endl;
    handler.sendCode(Protocol::ANS_GET_ART);
    
    try {
        Article art = db.getArticle(groupID, artID);
        
        cout << "Sending: ANS_ACK" << endl;
        handler.sendCode(Protocol::ANS_ACK);
        
        cout << "Sending: " << art.title << endl;
        handler.sendStringParam(art.title);
        
        cout << "Sending: " << art.author.name << endl;
        handler.sendStringParam(art.author.name);
        
        cout << "Sending: " << art.content << endl;
        handler.sendStringParam(art.content);
        
    } catch (NewsgroupDoesNotExistException&) {
        
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_NG_DOES_NOT_EXIST" << endl;
        handler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } catch (ArticleDoesNotExistException&) {
        
        cout << "Sending: ANS_NAK" << endl;
        handler.sendCode(Protocol::ANS_NAK);
        
        cout << "Sending: ERR_ART_DOES_NOT_EXIST" << endl;
        handler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    
    cout << "Sending: ANS_END" << endl;
    handler.sendCode(Protocol::ANS_END);
    
    cout << "=== End: Getting article ===" << endl << endl;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/*
 * The main
 */
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
    
    Database db;
    
    while (true) {
        Connection* conn = server.waitForActivity();
        if (conn != 0) {
            try {
                
                MessageHandler handler(conn);
                            
                unsigned char code = handler.recvCode();
                
                switch (code) {
                    case Protocol::COM_LIST_NG:
                        cout << "Received: COM_LIST_NG" << endl;
                        listNg(handler, db);
                        break;
                        
                    case Protocol::COM_CREATE_NG:
                        cout << "Received: COM_CREATE_NG" << endl;
                        createNg(handler, db);
                        break;
                        
                    case Protocol::COM_DELETE_NG:
                        cout << "Received: COM_DELETE_NG" << endl;
                        deleteNg(handler, db);
                        break;
                        
                    case Protocol::COM_LIST_ART:
                        cout << "Received: COM_LIST_ART" << endl;
                        listArt(handler, db);
                        break;
                        
                    case Protocol::COM_CREATE_ART:
                        cout << "Received: COM_CREATE_ART" << endl;
                        createArt(handler, db);
                        break;
                        
                    case Protocol::COM_DELETE_ART:
                        cout << "Received: COM_DELETE_ART" << endl;
                        deleteArt(handler, db);
                        break;
                        
                    case Protocol::COM_GET_ART:
                        cout << "Received: COM_GET_ART" << endl;
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
