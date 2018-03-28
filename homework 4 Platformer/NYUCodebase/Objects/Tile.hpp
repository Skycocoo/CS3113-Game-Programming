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
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"

class Tile: public Object{
public:
    // assume tiles start drawing at (0,0)
    Tile(): Object(){}

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

    Tile(const std::string& name): Object(&textured, 0), map(name){
        texture = LoadTexture((RESOURCE_FOLDER + name + ".png").c_str());

        vertices.clear();
        texCoords.clear();

        int spritex = 30, spritey = 30;
        float tilesize = 0.01;

        for (int y = 0; y < map.mapHeight; y++){
            for (int x = 0; x < map.mapWidth; x++){
                if (map.mapData[y][x] != -1){
                    float u = (float)(((int) map.mapData[y][x]) % spritex) / (float) spritex;
                    float v = (float)(((int) map.mapData[y][x]) / spritex) / (float) spritey;
                    float spriteWidth = 1.0f / (float) spritex;
                    float spriteHeight = 1.0f / (float) spritey;
                    vertices.insert(vertices.end(), {
                        tilesize * x, -tilesize * y,
                        tilesize * x, (-tilesize * y)-tilesize,
                        (tilesize * x)+tilesize, (-tilesize * y)-tilesize,
                        tilesize * x, -tilesize * y,
                        (tilesize * x)+tilesize, (-tilesize * y)-tilesize,
                        (tilesize * x)+tilesize, -tilesize * y
                    });

                    texCoords.insert(texCoords.end(), {
                        u, v,
                        u, v+(spriteHeight),
                        u+spriteWidth, v+(spriteHeight),
                        u, v,
                        u+spriteWidth, v+(spriteHeight),
                        u+spriteWidth, v
                    });
                }
            }
        }
    }



    void render(const Matrix& view = Matrix()){
        glUseProgram(program->programID);
        glBindTexture(GL_TEXTURE_2D, texture);

        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
        glEnableVertexAttribArray(program->positionAttribute);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
        glEnableVertexAttribArray(program->texCoordAttribute);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size()/2);
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
    }


private:
    FlareMap map;
};

#endif /* Tile_hpp */
