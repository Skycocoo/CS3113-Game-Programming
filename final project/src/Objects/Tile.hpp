// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#ifndef Tile_hpp
#define Tile_hpp

#include "Object.hpp"
#include "../Util/FlareMap.h"
#include "../setUp.hpp"

#include <string>
#include <iostream>
#include <set>

#include <chrono>

class Player;

class Tile: public Object{
public:
    // constructors, assume tiles start drawing at (0,0)
    Tile();
    // move copy constructor
    Tile(Tile&& rhs);
    // move assignment operator
    Tile& operator=(Tile&& rhs);

    Tile(const std::string& tex, const std::string& txt, float tilesize = 0.5);

    void loadMap(const std::string& txt);
    void loadType(const std::string& txt);

    void easeIn(float offTime, float fadeInTime);

    void render(const Matrix& view = Matrix());
    bool collide(Object& rhs) const;
    bool collide(Player& rhs) const;

private:
    FlareMap map;
    float tilesize;
    std::set<int> deco;
    std::set<int> trap;
    std::vector<int> end;
};

#endif /* Tile_hpp */
