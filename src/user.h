#ifndef USER_H
#define USER_H

#include <string>

struct User {
    
    /*
     * Copy constructor
     */
    User(const User& u) : name(u.name) {}
    
    /*
     * Constructor with string
     */
    User(std::string n) : name(n) {}
    
    std::string name;
};

#endif