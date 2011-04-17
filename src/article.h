#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include "user.h"

class Article {
public:
    
private:
    int ident;
    std::string title, content;
    User author;
};

#endif