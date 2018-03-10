// Yuxi Luo (yl4217), March 1, 2018
// CS3113 Game Programming

#include "XMLData.h"

std::ostream& operator<<(std::ostream& os, const XMLData& data){
    os << "x: " << data.x << " y: " << data.y << " width: " << data.width << " height: " << data.height << std::endl;
    return os;
}

XMLData::XMLData(){}

XMLData::XMLData(float x, float y, float width, float height): x(x), y(y), width(width), height(height){}

