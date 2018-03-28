// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#ifndef Tile_hpp
#define Tile_hpp

#include "Object.hpp"
#include "../Util/FlareMap.h"
#include "../setUp.hpp"

#include <string>
#include <iostream>

extern ShaderProgram textured;

class Tile: public Object{
public:
    // assume tiles start drawing at (0,0)
    Tile(): Object(){}

    Tile(GLuint texture, const std::string& name): Object(&textured, texture), map(name){
        textured = setTextured(name + ".png", texture);
    }

    // move semantics
    Tile(Tile&& rhs): map(rhs.map){
        rhs.map = FlareMap();
    }

    Tile& operator=(Tile&& rhs){
        map = rhs.map;
        rhs.map = FlareMap();
        return *this;
    }

private:
    FlareMap map;
};

#endif /* Tile_hpp */
