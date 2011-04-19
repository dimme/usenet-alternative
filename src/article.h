#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include "user.h"

struct Article {
    
    /*
     * Original constructor
     */
    Article(int i, User u, std::string t, std::string c) : ident(i),
                title(t),
                content(c),
                author(u) {} 
    
    /*
     * Copy constructor
     */
    Article(const Article& art) : ident(art.ident), 
            title(art.title), 
            content(art.content), 
            author(art.author) {}
    
    /*
     * It compares two articles for sorting
     */
    bool operator<(const Article& art);
    
    int ident; //It is unique and bigger than 0
    std::string title, content;
    User author;
};

struct ArticleDoesNotExistException {};

#endif