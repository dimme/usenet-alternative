#include <iostream>
#include <string>

#include "clientserver/protocol.h"
#include "message_handler.h"

using namespace std;
using protocol::Protocol;
using protocol::ProtocolViolationException;
using client_server::Connection;
using client_server::ConnectionClosedException;

/*
 * Printing help on how to use the client 
 */
void printHelp() {
    cout << "Available commands (Use Ctrl+C to exit):" << endl;
    cout << "* list_ng (Listing all available newsgroups)" << endl;
    cout << "* create_ng (Creating a new newsgroup)" << endl;
    cout << "* delete_ng (Deleting a newsgroup)" << endl;
    cout << "* list_art (Listing all articles for a newsgroup)" << endl;
    cout << "* create_art (Creating an article in a newsgroup)" << endl;
    cout << "* delete_art (Deleting an article from a newsgroup)" << endl;
    cout << "* get_art (Getting an article from a newsgroup)" << endl;
}

/*
 * Printing all the newsgroups
 */
void listNg(MessageHandler& handler) {
    handler.sendCode(Protocol::COM_LIST_NG);
    handler.sendCode(Protocol::COM_END);
    
    if (handler.recvCode() != Protocol::ANS_LIST_NG)
        throw ProtocolViolationException();
    
    int numOfGroups = handler.recvIntParam();
    
    for (int i = 0; i < numOfGroups; ++i) {
        int id = handler.recvIntParam();
        string title = handler.recvStringParam();
        
        cout << "GroupID: " << id << ", Name: " << title << endl;
    }
}

/*
 * Creating a new newsgroup
 */
void createNg(MessageHandler& handler) {
    cout << "Specify name: ";
    string title;
    getline(cin, title);
    getline(cin, title);
    
    handler.sendCode(Protocol::COM_CREATE_NG);
    handler.sendStringParam(title);
    handler.sendCode(Protocol::COM_END);
    
    if (handler.recvCode() != Protocol::ANS_CREATE_NG)
        throw ProtocolViolationException();
    
    unsigned char response = handler.recvCode();
    if (response == Protocol::ANS_ACK) {
        cout << "Newsgroup created successfully" << endl;
    } else if (response == Protocol::ANS_NAK) {
        if (handler.recvCode() == Protocol::ERR_NG_ALREADY_EXISTS)
            cout << "Newsgroup already exists" << endl;
        else 
            throw ProtocolViolationException();
    } else
        throw ProtocolViolationException();
}

/*
 * Deleting a newsgroup
 */
void deleteNg(MessageHandler& handler) {
    cout << "Specify newsgroup ID: ";
    int id;
    cin >> id;
    
    handler.sendCode(Protocol::COM_DELETE_NG);
    handler.sendIntParam(id);
    handler.sendCode(Protocol::COM_END);
    
    
    if (handler.recvCode() != Protocol::ANS_DELETE_NG)
        throw ProtocolViolationException();
    
    unsigned char response = handler.recvCode();
    if (response == Protocol::ANS_ACK) {
        cout << "Newsgroup deleted successfully" << endl;
    } else if (response == Protocol::ANS_NAK) {
        if (handler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST)
            cout << "Newsgroup does not exist" << endl;
        else 
            throw ProtocolViolationException();
    } else
        throw ProtocolViolationException();
}

/*
 * Listing the articles for a newsgroup
 */
void listArt(MessageHandler& handler) {
    cout << "Specify newsgroup ID: ";
    int id;
    cin >> id;
    
    handler.sendCode(Protocol::COM_LIST_ART);
    handler.sendIntParam(id);
    handler.sendCode(Protocol::COM_END);
    
    if (handler.recvCode() != Protocol::ANS_LIST_ART)
        throw ProtocolViolationException();
    
    unsigned char response = handler.recvCode();
    if (response == Protocol::ANS_ACK) {
        int numOfArticles = handler.recvIntParam();
        
        for (int i = 0; i < numOfArticles; ++i) {
            int id = handler.recvIntParam();
            string title = handler.recvStringParam();
            
            cout << "ArticleID: " << id << ", Title: " << title << endl;
        }
    } else if (response == Protocol::ANS_NAK) {
        if (handler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST)
            cout << "Newsgroup does not exist" << endl;
        else 
            throw ProtocolViolationException();
    } else
        throw ProtocolViolationException();
}

/*
 * Creating an article in a newsgroup
 */
void createArt(MessageHandler& handler) {
    cout << "Specify newsgroup ID: ";
    int id;
    cin >> id;
    
    cout << "Specify title: ";
    string title;
    getline(cin, title);
    getline(cin, title);
    
    cout << "Specify author: ";
    string author;
    getline(cin, author);
    
    cout << "Specify text: ";
    string text;
    getline(cin, text);
    
    handler.sendCode(Protocol::COM_CREATE_ART);
    handler.sendIntParam(id);
    handler.sendStringParam(title);
    handler.sendStringParam(author);
    handler.sendStringParam(text);
    handler.sendCode(Protocol::COM_END);
    
    if (handler.recvCode() != Protocol::ANS_CREATE_ART)
        throw ProtocolViolationException();
    
    unsigned char response = handler.recvCode();
    if (response == Protocol::ANS_ACK) {
        cout << "Article created successfully" << endl;
    } else if (response == Protocol::ANS_NAK) {
        if (handler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST)
            cout << "Newsgroup does not exist" << endl;
        else 
            throw ProtocolViolationException();
    } else
        throw ProtocolViolationException();
    
}

/*
 * Deleting an article from a newsgroup
 */
void deleteArt(MessageHandler& handler) {
    cout << "Specify newsgroup ID: ";
    int gid;
    cin >> gid;
    
    cout << "Specify article ID: ";
    int aid;
    cin >> aid;
    
    handler.sendCode(Protocol::COM_DELETE_ART);
    handler.sendIntParam(gid);
    handler.sendIntParam(aid);
    handler.sendCode(Protocol::COM_END);
    
    if (handler.recvCode() != Protocol::ANS_DELETE_ART)
        throw ProtocolViolationException();
    
    unsigned char response = handler.recvCode();
    if (response == Protocol::ANS_ACK) {
        cout << "Article deleted successfully" << endl;
    } else if (response == Protocol::ANS_NAK) {
        unsigned char response2 = handler.recvCode();
        if (response2 == Protocol::ERR_NG_DOES_NOT_EXIST)
            cout << "Newsgroup does not exist" << endl;
        else if (response2 == Protocol::ERR_ART_DOES_NOT_EXIST)
            cout << "Article does not exist" << endl;
        else
            throw ProtocolViolationException();
    } else
        throw ProtocolViolationException();
}

/*
 * Getting an article from a newsgroup
 */
void getArt(MessageHandler& handler) {
    cout << "Specify newsgroup ID: ";
    int gid;
    cin >> gid;
    
    cout << "Specify article ID: ";
    int aid;
    cin >> aid;
    
    handler.sendCode(Protocol::COM_GET_ART);
    handler.sendIntParam(gid);
    handler.sendIntParam(aid);
    handler.sendCode(Protocol::COM_END);
    
    if (handler.recvCode() != Protocol::ANS_GET_ART)
        throw ProtocolViolationException();
    
    unsigned char response = handler.recvCode();
    if (response == Protocol::ANS_ACK) {
        string title = handler.recvStringParam();
        string author = handler.recvStringParam();
        string text = handler.recvStringParam();
        
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Text: " << text << endl;
    } else if (response == Protocol::ANS_NAK) {
        unsigned char response2 = handler.recvCode();
        if (response2 == Protocol::ERR_NG_DOES_NOT_EXIST)
            cout << "Newsgroup does not exist" << endl;
        else if (response2 == Protocol::ERR_ART_DOES_NOT_EXIST)
            cout << "Article does not exist" << endl;
        else
            throw ProtocolViolationException();
    } else
        throw ProtocolViolationException();
}

/*
 * The main function of the client
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: client host-name port-number" << endl;
        exit(1);
    }
    
    Connection conn(argv[1], atoi(argv[2]));
    
    if (!conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    
    cout << "Type 'help' for help." << endl;
    cout << "# ";
    
    string cmd;
    while (cin >> cmd) {
        try {
            
            MessageHandler handler(&conn, true);
            
            if (cmd == "help")
                printHelp();
            else if (cmd == "list_ng")
                listNg(handler);
            else if (cmd == "create_ng")
                createNg(handler);
            else if (cmd == "delete_ng")
                deleteNg(handler);
            else if (cmd == "list_art")
                listArt(handler);
            else if (cmd == "create_art")
                createArt(handler);
            else if (cmd == "delete_art")
                deleteArt(handler);
            else if (cmd == "get_art")
                getArt(handler);
            else
                cout << "Unknown command! Type 'help' for a list of available commands." << endl;
            
            if (cmd != "help" && handler.recvCode() != Protocol::ANS_END)
                throw ProtocolViolationException();
            
            
            cout << endl << "# ";

        } catch (ConnectionClosedException&) {
            cerr << "Server closed down" << endl;
            exit(1);
        } catch (ProtocolViolationException&) {
            cout << "The protocol was violated by the server" << endl;
            exit(1);
        }
    }
}
