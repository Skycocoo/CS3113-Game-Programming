// Yuxi Luo (yl4217), March 1, 2018
// CS3113 Game Programming

#ifndef XMLData_h
#define XMLData_h

#include <iostream>

class XMLData {
public:
    friend std::ostream& operator<<(std::ostream& os, const XMLData& data);
    
    XMLData();
    XMLData(float x, float y, float width, float height);
    
    float x;
    float y;
    float width;
    float height;
};


#endif /* XMLData_h */
