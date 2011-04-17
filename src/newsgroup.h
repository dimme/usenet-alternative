#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <vector>
#include "article.h"

class Newsgroup {
public:
    
private:
    int ident; //It is unique and bigger than 0
    std::string name; //It is unique
    std::vector<Article> articles;
};

#endif