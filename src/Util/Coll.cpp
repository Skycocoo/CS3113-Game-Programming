#include "Coll.hpp"

void Coll::reset(){
    top = false;
    bottom = false;
    left = false;
    right = false;
}

std::ostream& operator<<(std::ostream& os, const Coll& c){
    os << std::boolalpha << "top: " << c.top << " bottom: " << c.bottom << " left: " << c.left << " right: " << c.right << "\n";
    return os;
}
