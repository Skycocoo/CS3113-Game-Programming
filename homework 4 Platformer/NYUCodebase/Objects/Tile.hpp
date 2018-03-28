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
    Tile(): Object::Object(){
        std::cout << "default Tile\n";
    }
    Tile(GLuint texture, const std::string& name): Object(&textured, texture){
        std::cout << "actual Tile\n";

        map.Load(name);
        std::cout << map.mapHeight << " " << map.mapWidth << std::endl;
        for(int y=0; y < map.mapHeight; y++) {
            for(int x=0; x < map.mapWidth; x++) {
              std::cout << map.mapData[y][x] << " ";
            }
            std::cout << std::endl;
        }
    }

    ~Tile(){
        std::cout << "after?\n";
    }

private:
    FlareMap map;

};

#endif /* Tile_hpp */
