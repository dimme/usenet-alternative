#ifndef DATABASE_H
#define DATABASE_H

#include "article.h"
#include "newsgroup.h"
#include "user.h"
#include <vector>
#include <iostream>
#include <fstream>

typedef std::vector<Article> ArticleList;
typedef std::pair<Newsgroup, ArticleList> GroupItem;
typedef std::vector<GroupItem> GroupList;


class Database {
public:
    Database();
    ~Database();
    
    void addNewsgroup(std::string name);
    void deleteNewsgroup(int ident);
    
    GroupList listNewsgroups();
    
    void addArticle(int groupID, 
                    User author, 
                    std::string title, 
                    std::string content);
    void deleteArticle(int groupID, int artID);
    
    ArticleList listArticles(int groupID);
    Article getArticle(int groupID, int artID);
    
private:
    GroupList newsgroups;
    
    void saveData(); // Implemented only by the disk-based database implementation
    void loadData(); // Same goes here
    std::string getString(std::ifstream& in, int length);
    GroupList::iterator findGroupByID(int groupID);
    GroupList::iterator findGroupByName(std::string groupName);
    int findBiggestGroupID();
    int findBiggestArticleID(ArticleList& list);
};

#endif
