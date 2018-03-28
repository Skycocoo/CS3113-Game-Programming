// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#ifndef Tile_hpp
#define Tile_hpp

#include "Object.hpp"
#include "../Util/FlareMap.h"

#include <string>
#include <iostream>

extern ShaderProgram textured;

class Tile: public Object{
public:
    // assume tiles start drawing at (0,0)
    Tile(): Object(){
        std::cout << "default Tile()\n";
    }

    Tile(GLuint texture, const FlareMap& map): Object(&textured, texture), map(map){
        std::cout << "actual Tile()\n";
    }

    void setTexture(GLuint tex){
        texture = tex;
    }

    // // move semantics
    // Tile(Tile&& rhs): map(rhs.map){
    //     rhs.map = FlareMap();
    // }
    //
    // Tile& operator=(Tile&& rhs){
    //     map = rhs.map;
    //     rhs.map = FlareMap();
    //     return *this;
    // }

    ~Tile(){
        std::cout << "~Tile()\n";
    }

private:
    FlareMap map;

};

#endif /* Tile_hpp */
