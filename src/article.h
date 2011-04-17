#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include "user.h"

class Article {
public:
    
private:
    int ident; //It is unique and bigger than 0
    std::string title, content;
    User author;
};

#endif