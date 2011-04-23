//This implements the disk-based database, still the same header file.
#include "database.h"
#include <sstream>

using namespace std;

/*
 * Constructor for the database loading data from disk
 */
Database::Database() {
    loadData();
}

/*
 * Destructor for the database doing nothing
 */
Database::~Database() { }

/*
 * Adds a new newsgroup to the database
 */
void Database::addNewsgroup(string name) {

    if(name != "" && findGroupByName(name) == newsgroups.end()) {
        Newsgroup group(findBiggestGroupID() + 1, name);
        ArticleList list;
        newsgroups.push_back(make_pair(group, list));
        saveData();
    } else 
        throw NewsgroupAlreadyExistsException();
}

/*
 * Deletes a existing newsgroup from the database
 */
void Database::deleteNewsgroup(int ident) {

    GroupList::iterator it = findGroupByID(ident);
    
    if(it != newsgroups.end() && it->first.name != "") {
        newsgroups.erase(it);
        saveData();
    } else
        throw NewsgroupDoesNotExistException();
}

/*
 * Returns a list with all existing newsgroups
 */
GroupList Database::listNewsgroups() {
    
    return newsgroups;
}

/*
 * Adds an article to a newsgroup
 */
void Database::addArticle(int groupID, User author, std::string title, std::string content) {
    
    GroupList::iterator it = findGroupByID(groupID);
    
    if(it != newsgroups.end() && it->first.name != "") {
        
        Article art(findBiggestArticleID(it->second)+1, author, title, content);
        it->second.push_back(art);
        saveData();
        
    } else
        throw NewsgroupDoesNotExistException();
}

/*
 * Deletes an article from a newsgroup
 */
void Database::deleteArticle(int groupID, int artID) {
    
    GroupList::iterator git = findGroupByID(groupID);
    
    if(git == newsgroups.end() || git->first.name == "")
        throw NewsgroupDoesNotExistException();
    
    ArticleList::iterator ait = git->second.begin();
    while (ait != git->second.end() && ait->ident != artID){
        ++ait;
    }
    
    if(ait == git->second.end())
        throw ArticleDoesNotExistException();
    
    git->second.erase(ait);
    saveData();
}

/*
 * Lists all articles for a newsgroup
 */
ArticleList Database::listArticles(int groupID) {
    
    GroupList::iterator git = findGroupByID(groupID);
    
    if(git == newsgroups.end())
        throw NewsgroupDoesNotExistException();
    
    return git->second;
}

/*
 * Returns an article from a newsgroup
 */
Article Database::getArticle(int groupID, int artID) {
    
    GroupList::iterator git = findGroupByID(groupID);
    
    if(git == newsgroups.end() || git->first.name == "")
        throw NewsgroupDoesNotExistException();
    
    ArticleList::iterator ait = git->second.begin();
    while (ait != git->second.end() && ait->ident != artID){
        ++ait;
    }
    
    if(ait == git->second.end())
        throw ArticleDoesNotExistException();
    
    return (*ait);
}

/*
 * Finds a newsgroup by ID in the list and returns an iterator to it
 */
GroupList::iterator Database::findGroupByID(int groupID) {
    
    GroupList::iterator it = newsgroups.begin();
    while (it != newsgroups.end() && it->first.ident != groupID)
        ++it;
    
    return it;
}

/*
 * Finds a newsgroup by name in the list and returns an iterator to it 
 */
GroupList::iterator Database::findGroupByName(string groupName) {
    
    GroupList::iterator it = newsgroups.begin();
    while (it != newsgroups.end() && it->first.name != groupName)
        ++it;
    
    return it;
}

/*
 * Finds and returns the biggest newsgroup ID
 */
int Database::findBiggestGroupID() {
    
    int max = 0;
    for(GroupList::iterator it = newsgroups.begin(); it != newsgroups.end(); ++it)
        if(it->first.ident > max) max = it->first.ident;
    
    return max;
}

/*
 * Finds and returns the bigger article ID
 */
int Database::findBiggestArticleID(ArticleList& list) {
    
    int max = 0;
    for(ArticleList::iterator it = list.begin(); it != list.end(); ++it)
        if(it->ident > max) max = it->ident;
    
    return max;
}

/*
 * Saves all the newsgroups and articles to the harddisk
 */
void Database::saveData() {
    ofstream out("data.db");
    
    out << newsgroups.size() << endl;
    for (GroupList::iterator git = newsgroups.begin(); git != newsgroups.end(); ++git) {
        
        out << git->first.ident << endl;
        out << git->first.name.size() << " " << git->first.name << endl;
        
        out << git->second.size() << endl;
        for (ArticleList::iterator ait = git->second.begin(); ait != git->second.end(); ++ait ) {            
            out << ait->ident << endl;
            out << ait->title.size() << " " << ait->title << endl;
            out << ait->author.name.size() << " " << ait->author.name << endl;
            out << ait->content.size() << " " << ait->content << endl;
        }   
    }
    
    out.close();
}

/*
 * Help function for Database::loadData();
 */
string Database::getString(ifstream& in, int length) {
    char * buffer = new char[length];
    in.ignore();
    in.read(buffer, length);
    
    string str(buffer);
    delete buffer;
    
    return str;
}

/*
 * Loads all the newsgroups and articles from the harddisk
 */
void Database::loadData() { 
    
    cout << "Initializing database..." << endl;
    ifstream in("data.db");
    
    int numOfGroups;
    in >> numOfGroups;
    
    for(int i = 0; i < numOfGroups; ++i) {
        
        int groupID;
        in >> groupID;
        cout << "Newsgroup ID: " << groupID << endl;
        
        int sizeOfGroupName;
        in >> sizeOfGroupName;
        
        string groupName = getString(in, sizeOfGroupName);
        cout << "Newsgroup name: " << groupName << endl;
   
        Newsgroup group(groupID, groupName);
        ArticleList artList;
        newsgroups.push_back(make_pair(group, artList));
        
        int numOfArticles;
        in >> numOfArticles;
        for(int j = 0; j < numOfArticles; ++j) {
            int articleID;
            in >> articleID;
            cout << "\tArticle ID: " << articleID << endl;
            
            int sizeOfTitle;
            in >> sizeOfTitle;
            
            string artTitle = getString(in, sizeOfTitle);
            cout << "\tArticle title: " << artTitle << endl;
            
            int sizeOfAuthorName;
            in >> sizeOfAuthorName;
            
            string artAuthorName = getString(in, sizeOfAuthorName);
            cout << "\tArticle author name: " << artAuthorName << endl;
            
            int sizeOfContent;
            in >> sizeOfContent;
            
            string artContent = getString(in, sizeOfContent);
            cout << "\tArticle content: " << artContent << endl << endl;
            
            // In a better implementation I would free this memory ;)
            User* author = new User(artAuthorName);
            Article* art = new Article(articleID, *author, artTitle, artContent);
            artList.push_back(*art);
        }
    }
    
    in.close();
    cout << "Database ready!" << endl << endl;
}
