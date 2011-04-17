#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <vector>
#include "article.h"

class Newsgroup {
public:
    
private:
    int ident;
    std::string name;
    std::vector<Article> articles;
};

#endif