#include "article.h"

bool Article::operator<(const Article& art) {
    return (ident < art.ident);
}
