// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#include "DynamicObj.hpp"
#include "Tile.hpp"

// constructors
DynamicObj::DynamicObj(): Object::Object(){}
DynamicObj::DynamicObj(ShaderProgram* program, GLuint texture, const glm::vec3& pos, const Tile* tile):
    Object(program, texture, pos), velo(0, 0, 0), fric(0.3, 0.3, 0.3), grav(0, -0.98, 0), acce(0, 0, 0), tile(tile) {}
    // if (texture == 0) program = &untextured;



// update velocity
void DynamicObj::updateVelo(float elapsed){
    velo += acce * elapsed;
    velo += grav * elapsed;
    // movement is influenced by friction
    lerp(velo, fric * elapsed);
}


// update position
void DynamicObj::update(float elapsed){
    updateVelo(elapsed);
    pos.x += velo.x * elapsed;
    pos.y += velo.y * elapsed;
    Object::update();
}


// setters

void DynamicObj::setTile(const Tile& t){
    tile = &t;
}

void DynamicObj::setVelo(float x, float y){
    velo.x = x;
    velo.y = y;
}





void DynamicObj::setVelo(float disp){
    velo.x += disp;
}

bool DynamicObj::satCollide(float elapsed, const Object& rhs) {
    DynamicObj::update(elapsed);

    float prevX = pos.x, prevY = pos.y;
    bool x = false, y = false;

    Object::update();
    bool result = Object::satCollide(rhs);
    Object::update();
    if (tile) result = tile->collide(*this) || result;

    if (result){
        if (prevY - pos.y != 0) y = true;
        if (prevX - pos.x != 0) x = true;
    }

    if (x) velo.x = 0;
    if (y) velo.y = 0;

    Object::update();
    return (x || y);
}

// only collide with tile
bool DynamicObj::collide(float elapsed) {
    bool result = false;
    updateVelo(elapsed);

    // x axis:
    pos.x += velo.x * elapsed;
    if (tile) result = tile->collide(*this) || result;
    if (result) velo.x = 0;

    // y axis:
    pos.y += velo.y * elapsed;
    // should also check collide with tile map
    if (tile) result = tile->collide(*this) || result;
    if (result) velo.y = 0;

    Object::update();

    return result;
}


bool DynamicObj::collide(float elapsed, const Object& rhs) {
    bool x = false, y = false;
    updateVelo(elapsed);

    // x axis:
    pos.x += velo.x * elapsed;
    x = Object::collide(rhs);
    if (tile) x = tile->collide(*this) || x;
    if (x) velo.x = 0;

    // y axis:
    pos.y += velo.y * elapsed;
    y = Object::collide(rhs);
    if (tile) y = tile->collide(*this) || y;
    if (y) velo.y = 0;

    Object::update();
    return (x || y);
}

// getters

const glm::vec3 DynamicObj::getVelo() const {
    return velo;
}
