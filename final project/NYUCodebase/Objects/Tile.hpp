// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#ifndef Tile_hpp
#define Tile_hpp

#include "Object.hpp"
#include "../Util/FlareMap.h"
#include "../setUp.hpp"

#include <string>
#include <iostream>

class Tile: public Object{
public:
    // constructors, assume tiles start drawing at (0,0)
    Tile();
    Tile(const std::string& tex, const std::string& txt, float tilesize = 0.5);
    // move copy constructor
    Tile(Tile&& rhs);

    // move assignment operator
    Tile& operator=(Tile&& rhs);

    void render(const Matrix& view = Matrix());
    bool collide(Object& rhs) const;

private:
    FlareMap map;
    float tilesize;

};

#endif /* Tile_hpp */
