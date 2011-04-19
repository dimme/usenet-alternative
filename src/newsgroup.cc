#include "newsgroup.h"

bool Newsgroup::operator<(const Newsgroup& group) {
    return (ident < group.ident);
}