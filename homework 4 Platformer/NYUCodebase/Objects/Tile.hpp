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

        vertices.clear();
        texCoords.clear();

        for(int y = 0; y < map.mapHeight; y++) {
            for(int x = 0; x < map.mapWidth; x++) {
                    float u = (float)(((int) map.mapData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                    float v = (float)(((int) map.mapData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
                    float spriteWidth = 1.0f/(float)SPRITE_COUNT_X;
                    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
                    vertices.insert(vertexData.end(), {
                        TILE_SIZE * x, -TILE_SIZE * y,
                        TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                        (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                        TILE_SIZE * x, -TILE_SIZE * y,
                        (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                        (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
            });
                    texCoords.insert(texCoordData.end(), {
                        u, v,
                        u, v+(spriteHeight),
                        u+spriteWidth, v+(spriteHeight),
                        u, v,
                        u+spriteWidth, v+(spriteHeight),
                        u+spriteWidth, v
            }
            });
        }


    }

    // move copy constructor
    Tile(Tile&& rhs): map(rhs.map){
        rhs.map = FlareMap();
    }

    // move assignment operator
    Tile& operator=(Tile&& rhs){
        map = rhs.map;
        rhs.map = FlareMap();
        return *this;
    }

private:
    FlareMap map;
};

#endif /* Tile_hpp */
