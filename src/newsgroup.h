#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <vector>
#include "article.h"

struct Newsgroup {
    
    /*
     * Original constructor
     */
    Newsgroup(int i, std::string n) : ident(i), name(n) {}

    /*
     * Copy constructor
     */
    Newsgroup(const Newsgroup& g) : ident(g.ident), name(g.name) {}
    
    /*
     * It compares two newsgroups for sorting
     */
    bool operator<(const Newsgroup& group);

    int ident; //It is unique and bigger than 0
    std::string name; //It is unique
};

struct NewsgroupDoesNotExistException {};
struct NewsgroupAlreadyExistsException {};

#endif