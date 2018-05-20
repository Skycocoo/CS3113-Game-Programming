#ifndef COLL_HPP
#define COLL_HPP

#include <iostream>

// collision flags for four sides
struct Coll{
    bool top = false, bottom = false, left = false, right = false;
    void reset();
};

std::ostream& operator<<(std::ostream& os, const Coll& c);

#endif
