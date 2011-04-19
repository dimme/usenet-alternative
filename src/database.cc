#include "database.h"


using namespace std;

/*
 * Constructor for the database
 */
Database::Database() {

}

/*
 * Destructor for the database
 */
Database::~Database() {
    
}

/*
 * Adds a new newsgroup to the database
 */
void Database::addNewsgroup(string name) {

    if(findGroupByName(name) == newsgroups.end()) {
        Newsgroup group(findBiggestGroupID() + 1, name);
        ArticleList list;
        newsgroups.push_back(make_pair(group, list));
    } else 
        throw NewsgroupAlreadyExistsException();
}

/*
 * Deletes a existing newsgroup from the database
 */
void Database::deleteNewsgroup(int ident) {

    GroupList::iterator it = findGroupByID(ident);
    
    if(it != newsgroups.end() && it->first.name != "")
        newsgroups.erase(it);
    else
        throw NewsgroupDoesNotExistException();
}

/*
 * Returns a list with all existing newsgroups
 */
bool groupComp (GroupItem i, GroupItem j) { return (i.first < j.first); }
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
        
    } else
        throw NewsgroupDoesNotExistException();
}

/*
 * Deletes an article from a newsgroup
 */
void Database::deleteArticle(int groupID, int artID) {

    GroupList::iterator git = findGroupByID(groupID);
    
    if(git != newsgroups.end() && git->first.name != "") {
        
        ArticleList::iterator ait = git->second.begin();
        while (ait != git->second.end()) {
            
            if(ait->ident == artID) {
                git->second.erase(ait);
            }
            ++ait;
        }
        
        if(ait == git->second.end())
            throw ArticleDoesNotExistException();
        
    } else
        throw NewsgroupDoesNotExistException();
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